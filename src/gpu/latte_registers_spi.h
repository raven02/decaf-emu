#pragma once
#include "types.h"
#include "utils/bitfield.h"
#include "latte_enum_spi.h"

namespace latte
{

struct SPI_INPUT_Z : Bitfield<SPI_INPUT_Z, uint32_t>
{
   BITFIELD_ENTRY(0, 1, bool, PROVIDE_Z_TO_SPI);
};

// Interpolator control settings
struct SPI_PS_IN_CONTROL_0 : Bitfield<SPI_PS_IN_CONTROL_0, uint32_t>
{
   BITFIELD_ENTRY(0, 6, uint32_t, NUM_INTERP);
   BITFIELD_ENTRY(8, 1, bool, POSITION_ENA);
   BITFIELD_ENTRY(9, 1, bool, POSITION_CENTROID);
   BITFIELD_ENTRY(10, 5, uint32_t, POSITION_ADDR);
   BITFIELD_ENTRY(15, 4, uint32_t, PARAM_GEN);
   BITFIELD_ENTRY(19, 7, uint32_t, PARAM_GEN_ADDR);
   BITFIELD_ENTRY(26, 2, SPI_BARYC_CNTL, BARYC_SAMPLE_CNTL);
   BITFIELD_ENTRY(28, 1, bool, PERSP_GRADIENT_ENA);
   BITFIELD_ENTRY(29, 1, bool, LINEAR_GRADIENT_ENA);
   BITFIELD_ENTRY(30, 1, bool, POSITION_SAMPLE);
   BITFIELD_ENTRY(31, 1, bool, BARYC_AT_SAMPLE_ENA);
};

// Interpolator control settings
struct SPI_PS_IN_CONTROL_1 : Bitfield<SPI_PS_IN_CONTROL_1, uint32_t>
{
   BITFIELD_ENTRY(0, 1, bool, GEN_INDEX_PIX);
   BITFIELD_ENTRY(1, 7, uint32_t, GEN_INDEX_PIX_ADDR);
   BITFIELD_ENTRY(8, 1, bool, FRONT_FACE_ENA);
   BITFIELD_ENTRY(9, 2, uint32_t, FRONT_FACE_CHAN);
   BITFIELD_ENTRY(11, 1, bool, FRONT_FACE_ALL_BITS);
   BITFIELD_ENTRY(12, 5, uint32_t, FRONT_FACE_ADDR);
   BITFIELD_ENTRY(17, 7, uint32_t, FOG_ADDR);
   BITFIELD_ENTRY(24, 1, bool, FIXED_PT_POSITION_ENA);
   BITFIELD_ENTRY(25, 5, uint32_t, FIXED_PT_POSITION_ADDR);
   BITFIELD_ENTRY(30, 1, bool, POSITION_ULC);
};

// PS interpolator setttings for parameter N
struct SPI_PS_INPUT_CNTL_N : Bitfield<SPI_PS_INPUT_CNTL_N, uint32_t>
{
   BITFIELD_ENTRY(0, 8, uint32_t, SEMANTIC);
   BITFIELD_ENTRY(8, 2, uint32_t, DEFAULT_VAL);
   BITFIELD_ENTRY(10, 1, bool, FLAT_SHADE);
   BITFIELD_ENTRY(11, 1, bool, SEL_CENTROID);
   BITFIELD_ENTRY(12, 1, bool, SEL_LINEAR);
   BITFIELD_ENTRY(13, 4, uint32_t, CYL_WRAP);
   BITFIELD_ENTRY(17, 1, bool, PT_SPRITE_TEX);
   BITFIELD_ENTRY(18, 1, bool, SEL_SAMPLE);
};

using SPI_PS_INPUT_CNTL_0 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_1 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_2 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_3 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_4 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_5 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_6 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_7 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_8 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_9 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_10 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_11 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_12 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_13 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_14 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_15 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_16 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_17 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_18 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_19 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_20 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_21 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_22 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_23 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_24 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_25 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_26 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_27 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_28 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_29 = SPI_PS_INPUT_CNTL_N;
using SPI_PS_INPUT_CNTL_30 = SPI_PS_INPUT_CNTL_N;

// Vertex Shader output configuration
struct SPI_VS_OUT_CONFIG : Bitfield<SPI_VS_OUT_CONFIG, uint32_t>
{
   BITFIELD_ENTRY(0, 1, bool, VS_PER_COMPONENT);
   BITFIELD_ENTRY(1, 5, uint32_t, VS_EXPORT_COUNT);
   BITFIELD_ENTRY(8, 1, bool, VS_EXPORTS_FOG);
   BITFIELD_ENTRY(9, 5, uint32_t, VS_OUT_FOG_VEC_ADDR);
};

// Vertex Shader output semantic mapping
struct SPI_VS_OUT_ID_N : Bitfield<SPI_VS_OUT_ID_N, uint32_t>
{
   BITFIELD_ENTRY(0, 8, uint32_t, SEMANTIC_0);
   BITFIELD_ENTRY(8, 8, uint32_t, SEMANTIC_1);
   BITFIELD_ENTRY(16, 8, uint32_t, SEMANTIC_2);
   BITFIELD_ENTRY(24, 8, uint32_t, SEMANTIC_3);
};

using SPI_VS_OUT_ID_0 = SPI_VS_OUT_ID_N;
using SPI_VS_OUT_ID_1 = SPI_VS_OUT_ID_N;
using SPI_VS_OUT_ID_2 = SPI_VS_OUT_ID_N;
using SPI_VS_OUT_ID_3 = SPI_VS_OUT_ID_N;
using SPI_VS_OUT_ID_4 = SPI_VS_OUT_ID_N;
using SPI_VS_OUT_ID_5 = SPI_VS_OUT_ID_N;
using SPI_VS_OUT_ID_6 = SPI_VS_OUT_ID_N;
using SPI_VS_OUT_ID_7 = SPI_VS_OUT_ID_N;
using SPI_VS_OUT_ID_8 = SPI_VS_OUT_ID_N;
using SPI_VS_OUT_ID_9 = SPI_VS_OUT_ID_N;

} // namespace latte
