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

const uint8 GAME_COLS(10U);
const uint8 GAME_ROWS(9U);

#define RED(col)   ((col >> 24) & 0xFF)
#define GREEN(col) ((col >> 16) & 0xFF)
#define BLUE(col)  ((col >> 8)  & 0xFF)
#define ALPHA(col) ((col >> 0)  & 0xFF)

namespace envcolors
{
    const uint32 EC_CBLUE(0x58B8F8FF);
    const uint32 EC_WHITE(0xF8F8F8FF);
    const uint32 EC_BLACK(0x181818FF);
    const uint32 EC_PALET(0xB888F8FF);
    const uint32 EC_VIRID(0x93F81FFF);
    const uint32 EC_PEWTR(0x908F77FF);
    const uint32 EC_CERUL(0x2840F8FF);
    const uint32 EC_VERML(0xF89800FF);
    const uint32 EC_LAVEN(0xC820F8FF);
    const uint32 EC_CINAB(0xF84040FF);
    const uint32 EC_CAVES(0xB84000FF);
}