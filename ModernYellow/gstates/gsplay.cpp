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
#include "../game/sprite.h"
#include "../game/player.h"
#include "../portcommon.h"
#include <SDL_render.h>
#include <SDL_log.h>
#include <SDL_gfxPrimitives.h>

extern SDL_Renderer* g_renderer;
extern uint32 g_tileSize;
extern uint32 g_width;
extern uint32 g_height;
extern uint32 g_scale;

int32 xo = 0;
int32 yo = 0;

/* ==============
   Public Methods
   ============== */
GSPlay::GSPlay():
    GState()
{        
    auto pAtlas = castResToTex(resmanager.loadResource("tilemaps/overworldmap.png", RT_TEXTURE));
    
    m_pLevel = std::make_unique<Level>("tpallet");

    m_pPlayer = std::make_unique<Player>(
        m_pLevel->getTileRC(8, 14),
        m_pLevel,
        pAtlas);
    
}

GSPlay::~GSPlay()
{
}

void GSPlay::update()
{           
    m_pPlayer->update();

    xo = g_width / 2  - m_pPlayer->getX() - g_tileSize;
    yo = g_height / 2 - m_pPlayer->getY() - g_tileSize;

    m_pLevel->setOffset(xo, yo);
    m_pPlayer->setOffset(xo, yo);
}

void GSPlay::render()
{        
    m_pLevel->render();       
    m_pPlayer->render();
}
