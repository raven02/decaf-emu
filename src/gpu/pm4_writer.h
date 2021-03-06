#pragma once
#include <gsl.h>
#include "pm4.h"
#include "pm4_buffer.h"
#include "pm4_format.h"
#include "latte_registers.h"
#include "utils/virtual_ptr.h"
#include "utils/log.h"

namespace pm4
{

class PacketWriter
{
public:
   PacketWriter(type3::IT_OPCODE op)
   {
      mBuffer = pm4::getBuffer(1);

      if (mBuffer) {
         mSaveSize = mBuffer->curSize;

         auto header = type3::Header::get(0)
            .type().set(Header::Type3)
            .opcode().set(op);

         *reinterpret_cast<type3::Header *>(&mBuffer->buffer[mBuffer->curSize++]) = header;
      }
   }

   ~PacketWriter()
   {
      if (mBuffer) {
         auto size = mBuffer->curSize - mSaveSize;
         if (size < 2) {
            // This really is an std::logic_error, but we can't throw from destructors.
            gLog->warn("Encoded a pm4 type3 packet with size < 2.");
         }

         // Update header
         auto header = *reinterpret_cast<type3::Header *>(&mBuffer->buffer[mSaveSize]);

         header = header
            .size().set(size - 2);

         *reinterpret_cast<type3::Header *>(&mBuffer->buffer[mSaveSize]) = header;

         // Swap to big endian
         for (auto i = mSaveSize; i < mBuffer->curSize; ++i) {
            mBuffer->buffer[i] = byte_swap(mBuffer->buffer[i]);
         }
      }
   }

   bool valid()
   {
      return !!mBuffer;
   }

   // Write one word
   PacketWriter &operator()(uint32_t value)
   {
      checkSize(1);
      mBuffer->buffer[mBuffer->curSize++] = value;
      return *this;
   }

   // Write one float
   PacketWriter &operator()(float value)
   {
      checkSize(1);
      mBuffer->buffer[mBuffer->curSize++] = bit_cast<uint32_t>(value);
      return *this;
   }

   // Write a virtual ptr as one word
   template<typename Type>
   PacketWriter &operator()(virtual_ptr<Type> value)
   {
      checkSize(1);
      mBuffer->buffer[mBuffer->curSize++] = value.getAddress();
      return *this;
   }

   // Write a list of words
   template<typename Type>
   PacketWriter &operator()(const gsl::span<Type> &values)
   {
      auto size = gsl::narrow_cast<uint32_t>(((values.size() * sizeof(Type)) + 3) / 4);
      checkSize(size);
      memcpy(&mBuffer->buffer[mBuffer->curSize], values.data(), size * sizeof(uint32_t));
      mBuffer->curSize += size;
      return *this;
   }

   // Write one word as a REG_OFFSET
   PacketWriter &REG_OFFSET(latte::Register value, latte::Register base)
   {
      auto offset = static_cast<uint32_t>(value) - static_cast<uint32_t>(base);
      checkSize(1);
      mBuffer->buffer[mBuffer->curSize++] = offset / 4;
      return *this;
   }

   // Write one word as a CONST_OFFSET
   PacketWriter &CONST_OFFSET(uint32_t value)
   {
      checkSize(1);
      mBuffer->buffer[mBuffer->curSize++] = value;
      return *this;
   }

   // Write one word as a size (N - 1)
   template<typename Type>
   PacketWriter &size(Type value)
   {
      checkSize(1);
      mBuffer->buffer[mBuffer->curSize++] = static_cast<uint32_t>(value) - 1;
      return *this;
   }

private:
   void checkSize(uint32_t size)
   {
      if (mBuffer->curSize + size <= mBuffer->maxSize) {
         return;
      }

      // Reset old buffer
      auto oldBuffer = mBuffer;
      auto oldSize = oldBuffer->curSize;
      oldBuffer->curSize = mSaveSize;

      // Copy to new buffer
      auto newBuffer = pm4::flushBuffer(oldBuffer);
      auto saveSize = newBuffer->curSize;

      if ((newBuffer->curSize + size) > newBuffer->maxSize) {
         throw std::logic_error("PM4 packet is wayy too big son");
      }

      if (oldSize > mSaveSize) {
         memcpy(&newBuffer->buffer[newBuffer->curSize], &oldBuffer->buffer[mSaveSize], 4 * (oldSize - mSaveSize));
         newBuffer->curSize += oldSize - mSaveSize;
      }

      mBuffer = newBuffer;
      mSaveSize = saveSize;
   }

private:
   pm4::Buffer *mBuffer;
   uint32_t mSaveSize;
};

template<typename Type>
void write(const Type &value)
{
   PacketWriter writer { Type::Opcode };

   if (writer.valid()) {
      // Don't judge me
      const_cast<Type &>(value).serialise(writer);
   }
}

} // namespace pm4
