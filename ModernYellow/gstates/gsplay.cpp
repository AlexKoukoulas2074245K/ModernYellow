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

int32 globXOffset = 0;
int32 globYOffset = 0;

/* ==============
   Public Methods
   ============== */
GSPlay::GSPlay():
    GState()
{            
    auto pAtlas = castResToTex(resmanager.loadResource("tilemaps/overworldmap.png", RT_TEXTURE));
    
    m_pLevel = std::shared_ptr<Level>(new Level("opallet", pAtlas));    
    
    m_pPlayer = std::make_unique<Player>(
        m_pLevel->getTileRC(8, 14),        
        m_pLevel,
        pAtlas);
    
    m_pLevel->getNPCData(pAtlas, m_npcs);       
}

GSPlay::~GSPlay()
{
}

void GSPlay::update()
{           
    m_pPlayer->update();

    globXOffset = g_width / 2  - m_pPlayer->getX() - g_tileSize;
    globYOffset = g_height / 2 - m_pPlayer->getY() - g_tileSize;

    m_pLevel->setOffset(globXOffset, globYOffset);    
    m_pPlayer->setOffset(globXOffset, globYOffset);

    for (const auto& npc: m_npcs)
    {
        npc->setOffset(globXOffset, globYOffset);
    }
}

void GSPlay::render()
{        
    m_pLevel->render();       
    
    for (const auto& npc: m_npcs)
    {
        npc->render();
    }

    m_pPlayer->render();
}
