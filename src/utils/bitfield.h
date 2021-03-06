#pragma once
#include <cassert>
#include <type_traits>

template<typename ParentType, typename Type>
struct Bitfield
{
   using StorageType = Type;
   using BitfieldType = ParentType;

   template<typename ValueType, unsigned Position, unsigned Bits>
   struct Field
   {
      static const auto MinValue = static_cast<ValueType>(0);
      static const auto MaxValue = static_cast<ValueType>((1ull << (Bits)) - 1);
      static const auto Mask = (static_cast<StorageType>((1ull << (Bits)) - 1)) << (Position);

      constexpr ValueType get() const
      {
         return static_cast<ValueType>((parent.value & Mask) >> (Position));
      }

      inline BitfieldType set(ValueType value)
      {
         assert(value >= MinValue);
         assert(value <= MaxValue);
         parent.value &= ~Mask;
         parent.value |= static_cast<StorageType>(value) << (Position);
         return parent;
      }

      operator ValueType() const
      {
         return get();
      }

      BitfieldType parent;
   };

   // Specialise for bool because of compiler warnings for static_cast<bool>(int)
   template<unsigned Position, unsigned Bits>
   struct Field<bool, Position, Bits>
   {
      static const auto Mask = (static_cast<StorageType>((1ull << (Bits)) - 1)) << (Position);

      constexpr bool get() const
      {
         return !!((parent.value & Mask) >> (Position));
      }

      inline BitfieldType set(bool value)
      {
         parent.value &= ~Mask;
         parent.value |= (static_cast<StorageType>(value ? 1 : 0)) << (Position);
         return parent;
      }

      operator bool() const
      {
         return get();
      }

      BitfieldType parent;
   };

   static BitfieldType get(StorageType value)
   {
      BitfieldType bitfield;
      bitfield.value = value;
      return bitfield;
   }

   StorageType value;
   static_assert(std::is_integral<StorageType>::value, "Bitfield storage type must be an integer type");
};

#define BITFIELD_ENTRY(Pos, Size, Type, Name) inline Field<Type, Pos, Size> Name() const { return { *this }; }
