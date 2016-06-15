/* ======================
   Date: 30/5/2016
   Author: Alex Koukoulas
   ====================== */

#include "gsplay.h"
#include "../mydef.h"
#include "../resources/sresmanager.h"
#include "../resources/textureresource.h"
#include "../sinputhandler.h"
#include "../game/level.h"
#include "../portcommon.h"
#include <SDL_render.h>
#include <SDL_log.h>

extern SDL_Renderer* g_renderer;

/* ==============
   Public Methods
   ============== */
GSPlay::GSPlay():
    GState(),
    m_level(nullptr)
{    
    m_level = std::make_unique<Level>("tpallet");
}

GSPlay::~GSPlay()
{
}

void GSPlay::update()
{
    
}

void GSPlay::render()
{        
    m_level->render();   
}
