/* ======================
   Date: 30/5/2016
   Author: Alex Koukoulas
   ====================== */

#include "gsplay.h"
#include "../resources/sresmanager.h"
#include "../resources/textureresource.h"
#include "../sinputhandler.h"
#include "../game/level.h"
#include "../game/player.h"
#include "../game/npc.h"
#include "../game/tile.h"
#include "../portcommon.h"
#include "../mixer.h"

#include <SDL_render.h>
#include <SDL_log.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_timer.h>

extern pRenderer_t g_pRenderer;
extern pMixer_t g_pMixer;

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
    GState(),
    m_uiActions(false)
{            
    auto pAtlas = castResToTex(resmanager.loadResource("tilemaps/overworldmap.png", RT_TEXTURE));
    
    auto start = SDL_GetTicks();
    m_pLevel = std::make_shared<Level>("oviridian", pAtlas);        
    m_pLevel->loadNPCData();

    m_pPlayer = std::make_unique<Player>(
        m_pLevel->getTileRC(28, 34),        
        Direction::DIR_UP,
        m_pLevel,
        pAtlas);
    
    m_pLevel->establishNewColor();
    SDL_Log(std::to_string(SDL_GetTicks() - start).c_str());
}

GSPlay::~GSPlay()
{
}

void GSPlay::update()
{   
    m_pLevel->update();
    m_pPlayer->update();

    if (m_pPlayer->hasUIDialog() != m_uiActions)
    {
        m_uiActions = m_pPlayer->hasUIDialog();
        m_pLevel->setFrozenNpcs(m_uiActions);
    }    

    globXOffset = g_width / 2  - m_pPlayer->getX() - g_tileSize;
    globYOffset = g_height / 2 - m_pPlayer->getY() - g_tileSize / 2;

    m_pLevel->setOffset(globXOffset, globYOffset);    
    m_pPlayer->setOffset(globXOffset, globYOffset); 

    static bool isInit = false;
    static int delay = 3;
    if (ihandler.isKeyTapped(K_SELECT))
    {        
        SDL_Log("INIt");
        m_pLevel->initWildEncounterEffect(); 
        isInit = true;        
        g_pMixer->playAudio("ambient/wildpokemon.mp3", false, false);
    }
    if (isInit && !--delay)
    {
        delay = 3;
        m_pLevel->playWildEncounterEffect();
    }
}

void GSPlay::render()
{        
    m_pLevel->render();           
    m_pPlayer->render();

    if (!m_uiActions)
    {
        m_pLevel->renderEncOccTiles();
    }    
}
