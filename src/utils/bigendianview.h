#pragma once
#include <cstring>
#include <gsl.h>
#include <string>
#include "utils/byte_swap.h"

class BigEndianView
{
public:
   BigEndianView(const void *buffer, std::size_t size) :
      mBuffer(reinterpret_cast<const uint8_t*>(buffer)), mSize(size), mOffset(0)
   {
   }

   BigEndianView(const gsl::span<uint8_t> &view) :
      mBuffer(reinterpret_cast<const uint8_t*>(view.data())), mSize(view.size()), mOffset(0)
   {
   }

   BigEndianView(const gsl::span<char> &view) :
      mBuffer(reinterpret_cast<const uint8_t*>(view.data())), mSize(view.size()), mOffset(0)
   {
   }

   template<typename Type>
   void read(Type &value)
   {
      value = byte_swap(*reinterpret_cast<const Type*>(mBuffer + mOffset));
      mOffset += sizeof(Type);
   }

   template<typename Type>
   Type read()
   {
      Type value = byte_swap(*reinterpret_cast<const Type*>(mBuffer + mOffset));
      mOffset += sizeof(Type);
      return value;
   }

   template<typename Type, size_t count>
   void read(Type(&chars)[count])
   {
      std::memcpy(chars, mBuffer + mOffset, sizeof(Type) * count);
      mOffset += sizeof(Type) * count;
   }

   template<typename Type>
   void read(Type *buffer, size_t count)
   {
      std::memcpy(buffer, mBuffer + mOffset, sizeof(Type) * count);
      mOffset += sizeof(Type) * count;
   }

   template<typename Type>
   void read(const gsl::span<Type> &arr)
   {
      for (auto i = 0u; i < arr.size(); ++i) {
         read<Type>(arr[i]);
      }
   }

   std::string readNullTerminatedString()
   {
      const char *str = reinterpret_cast<const char*>(mBuffer) + mOffset;
      mOffset += strlen(str) + 1;
      return str;
   }

   template<typename Type>
   const Type *readRaw(size_t count)
   {
      auto ptr = reinterpret_cast<const Type*>(mBuffer + mOffset);
      mOffset += sizeof(Type) * count;
      return ptr;
   }

   void seek(size_t pos)
   {
      mOffset = pos;
   }

   bool eof()
   {
      return mOffset >= mSize;
   }

private:
   const uint8_t *mBuffer;
   std::size_t mSize;
   std::size_t mOffset;
};
