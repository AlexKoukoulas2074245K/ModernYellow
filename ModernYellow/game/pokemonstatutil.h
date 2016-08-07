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
        const int8 level, 
        const int16 baseStat, 
        const uint8 iv,
        const uint16 ev)
    {
        const int32 evFactor = (int32) (floor((int32) ((int32) (ceil((int32) (sqrt(ev)))) / 4)));
        const int32 numFactor = (int32) ((baseStat + iv) * 2 + evFactor) * level;
        return (int16)((int32)(floor((int16) (numFactor / 100))) + 5);
    }

    int16 calculateHPStat(
        const int8 level,
        const int16 baseStat,
        const uint8 iv,
        const uint16 ev)
    {
        const int32 evFactor = (int32)(floor((int32)((int32)(ceil((int32)(sqrt(ev))))/4)));
        const int32 numFactor = (int32)((baseStat + iv) * 2 + evFactor) * level;
        return (int16)((int32)(floor((int32)(numFactor / 100))) + level + 10);
    }

    float getPercentDepleted(const int16 currHp, const int16 maxHp)
    {
        return 1.0f - static_cast<float>(currHp) / static_cast<float>(maxHp);
    }
}