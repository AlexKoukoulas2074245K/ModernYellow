/* ======================
   Date: 31/7/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once
#include "../mydef.h"

#include <math.h>

namespace stat_utils
{
    int16 calculateStatOtherThanHP(
        const uint8 level, 
        const int16 baseStat, 
        const uint8 iv,
        const uint16 ev)
    {
        const int16 evFactor = (int16) (floor((int16) ((int16) (ceil((int16) (sqrt(ev)))) / 4)));
        const int16 numFactor = (int16) ((baseStat + iv) * 2 + evFactor) * level;
        return (int16) (floor((int16) (numFactor / 100))) + 5;
    }

    int16 calculateHPStat(
        const uint8 level,
        const int16 baseStat,
        const uint8 iv,
        const uint16 ev)
    {
        const int16 evFactor = (int16)(floor((int16)((int16)(ceil((int16)(sqrt(ev))))/4)));
        const int16 numFactor = (int16)((baseStat + iv) * 2 + evFactor) * level;
        return (int16)(floor((int16)(numFactor / 100))) + level + 10;
    }
}