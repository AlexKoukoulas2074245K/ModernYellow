/* ======================
   Date: 30/5/2016
   Author: Alex Koukoulas
   ====================== */

#include "gstate.h"

/* ==============
   Public Methods
   ============== */
GState::GState()
{
    m_finished = false;    
}

GState::~GState(){}

bool GState::isFinished() const
{
    return m_finished;
}