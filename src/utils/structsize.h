#pragma once
#include <cstddef>

// Workaround weird macro concat ## behaviour
#define PP_CAT(a, b) PP_CAT_I(a, b)
#define PP_CAT_I(a, b) PP_CAT_II(~, a ## b)
#define PP_CAT_II(p, res) res

// Ensure our structs are correct size & offsets to match WiiU
#define CHECK_SIZE(Type, Size) \
   static_assert(sizeof(Type) == Size, \
                 #Type " must be " #Size " bytes")

#define CHECK_OFFSET(Type, Offset, Field) \
   static_assert(offsetof(Type, Field) == Offset, \
                 #Type "::" #Field " must be at offset " #Offset)

#define CHECK_MEMBER_OFFSET_START \
   void PP_CAT(__verifyMemberOffsets, __COUNTER__) () {

#define CHECK_MEMBER_OFFSET_END \
   }

// TODO: Figure out how to implement this, might be impossible?
#define CHECK_BIT_OFFSET(Type, Offset, Field)

// Allow us to easily add UNKNOWN / PADDING bytes into our structs,
// generates unique variable names using __COUNTER__
#define UNKNOWN(Size) char PP_CAT(__unk, __COUNTER__) [Size]
#define PADDING(Size) UNKNOWN(Size)

#define UNKNOWN_ARGS void
#define UNKNOWN_SIZE(x) //x
