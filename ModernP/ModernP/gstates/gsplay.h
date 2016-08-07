/* ======================
   Date: 30/5/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once

#include "gstate.h"

class GSPlay final: public GState
{
public:

    GSPlay();

    ~GSPlay();

    void update() override;

    void render() override;
    
};