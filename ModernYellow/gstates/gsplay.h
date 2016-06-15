/* ======================
   Date: 30/5/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once

#include "gstate.h"
#include <memory>

class Level;
class GSPlay final: public GState
{
public:

    GSPlay();

    ~GSPlay();

    void update() override;

    void render() override;

private:

    std::unique_ptr<Level> m_level;

};