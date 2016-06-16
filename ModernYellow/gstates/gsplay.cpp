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
    GState(),
    m_level(nullptr)
{    
    m_level = std::make_unique<Level>("tpallet");
    
    auto pAtlas = castResToTex(resmanager.loadResource("tilemaps/overworldmap.png", RT_TEXTURE));
    
    m_player = std::make_unique<Sprite>(64, 64, 6 * DEFAULT_TILE_SIZE, 14 * DEFAULT_TILE_SIZE, pAtlas);
    
}

GSPlay::~GSPlay()
{
}

void GSPlay::update()
{           
    m_player->update();

    xo = g_width / 2  - m_player->getX() - g_tileSize;
    yo = g_height / 2 - m_player->getY() - g_tileSize;

    m_level->setOffset(xo, yo);
    m_player->setOffset(xo, yo);
}

void GSPlay::render()
{        
    m_level->render();       
    m_player->render();
}
