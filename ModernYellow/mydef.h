/* ======================
   Date: 30/5/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once

#include <cstdint>

using int8  = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

using uint8  = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

const uint32 BITS_PER_PIXEL = 32U;
const uint32 DEFAULT_TILE_SIZE = 16U;

#define RED(col)   ((col >> 0)  & 0xFF)
#define GREEN(col) ((col >> 8)  & 0xFF) 
#define BLUE(col)  ((col >> 16) & 0xFF)
#define ALPHA(col) ((col >> 24) & 0xFF)

namespace envcolors
{
    const uint32 EC_CBLUE(0xFFF8B858);
    const uint32 EC_WHITE(0xFFF8F8F8);
    const uint32 EC_BLACK(0xFF181818);
    const uint32 EC_PALET(0xFFF888B8);
    const uint32 EC_VIRID(0xFF1FF893);
    const uint32 EC_PEWTR(0xFF778F90);
    const uint32 EC_CERUL(0xFFF84028);
    const uint32 EC_VERML(0xFF0098F8);
    const uint32 EC_LAVEN(0xFFF820C8);
    const uint32 EC_CINAB(0xFF4040F8);
    const uint32 EC_CAVES(0xFF0040B8);
}

enum Direction
{
    DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFT
};
