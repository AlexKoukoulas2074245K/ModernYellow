/* ======================
   Date: 31/7/2016
   Author: Alex Koukoulas
   ====================== */

#include "battlecontroller.h"
#include "battlefsm/fsmstatedarkintro.h"
#include "../uicomps/uicomponent.h"
#include "../../resources/sresmanager.h"
#include "../../resources/textureresource.h"
#include "../pokemon.h"

extern pRenderer_t g_pRenderer;
extern uint32 g_height;
extern uint32 g_scale;

/* ==============
   Public Methods
   ============== */
BattleController::BattleController(
    const pokemonParty_t& localPokemon,
    const pokemonParty_t& enemyPokemon,
    const BattleType& battleType):
   
    m_localPokemon(localPokemon),
    m_enemyPokemon(enemyPokemon),
    m_battleType(battleType),
    m_textboxTexture(castResToTex(resmanager.loadResource("misctex/textbox.png", RT_TEXTURE))),
    m_normalTrainerAtlas(castResToTex(resmanager.loadResource("tilemaps/traineratlas.png", RT_TEXTURE))),
    m_darkTrainerAtlas(castResToTex(resmanager.loadResource("tilemaps/dtraineratlas.png", RT_TEXTURE))),
    m_activeState(std::make_unique<FSMStateDarkIntro>(
        m_normalTrainerAtlas,
        m_darkTrainerAtlas,
        m_uiComponentStack,
        localPokemon,
        enemyPokemon,
        battleType == BT_ENCOUNTER))
{
    
}

BattleController::~BattleController(){}

void BattleController::update()
{
    m_activeState->update();
    if (m_activeState->isFinished())
    {
        m_activeState = m_activeState->getSuccessor();
    }
}

void BattleController::render()
{
    SDL_SetRenderDrawColor(g_pRenderer.get(),
        RED(envcolors::EC_WHITE),
        GREEN(envcolors::EC_WHITE),
        BLUE(envcolors::EC_WHITE),
        ALPHA(envcolors::EC_WHITE));
    SDL_RenderClear(g_pRenderer.get());
        
    SDLRender(
        g_pRenderer, 
        m_textboxTexture->getTexture().get(),
        0,
        g_height - m_textboxTexture->getScaledHeight(),
        m_textboxTexture->getScaledWidth(),
        m_textboxTexture->getScaledHeight());
    
    m_activeState->render();

}