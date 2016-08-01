/* ======================
   Date: 31/7/2016
   Author: Alex Koukoulas
   ====================== */

#include "fsmstatedarkintro.h"
#include "fsmstateintro.h"
#include "../../pokemon.h"

extern pRenderer_t g_pRenderer;
extern uint32 g_scale;
extern uint32 g_width;
extern uint32 g_height;

/* ==================
   Internal Constants
   ================== */
static const int32 DEFAULT_INTRO_MOVING_SPEED = 2;

/* ==============
   Public Methods
   ============== */

FSMStateDarkIntro::FSMStateDarkIntro(
    const std::shared_ptr<TextureResource> normalTrainerAtlas,
    const std::shared_ptr<TextureResource> darkTrainerAtlas,
    BattleController::uiComponentStack_t& uiComponents,
    const BattleController::pokemonParty_t& localPokemon,
    const BattleController::pokemonParty_t& enemyPokemon,
    const bool isWildEncounter):
    
    FSMState(
        normalTrainerAtlas, 
        darkTrainerAtlas, 
        uiComponents,
        localPokemon,
        enemyPokemon, 
        isWildEncounter)         
{
    m_playerDarkTexture = darkTrainerAtlas->getSubTexture(
        PLAYER_PORTRAIT_TU, 
        PLAYER_PORTRAIT_TV, 
        TRAINER_PORTRAIT_WIDTH, 
        TRAINER_PORTRAIT_HEIGHT);        

    if (isWildEncounter)    
        m_opponentDarkTexture = castResToTex(
            resmanager.loadResource("pkmnfront/" + m_enemyPokemon[0]->getName() + "_DARK.png", RT_TEXTURE));
    else
        m_opponentDarkTexture = darkTrainerAtlas->getSubTexture(
            0,
            0,
            TRAINER_PORTRAIT_WIDTH, 
            TRAINER_PORTRAIT_HEIGHT);

    m_playerTexturePosition = {
        g_width, 
        g_height - TRAINER_PORTRAIT_HEIGHT * g_scale - DEFAULT_TEXTBOX_HEIGHT * g_scale,
        TRAINER_PORTRAIT_WIDTH * g_scale, 
        TRAINER_PORTRAIT_HEIGHT * g_scale};

    m_opponentTexturePosition = {
        -TRAINER_PORTRAIT_WIDTH * g_scale + 4 * g_scale, 
        0, 
        TRAINER_PORTRAIT_WIDTH * g_scale, 
        TRAINER_PORTRAIT_HEIGHT * g_scale};
}

void FSMStateDarkIntro::update()
{
    if (m_playerTexturePosition.x > LOCAL_ACTOR_X * static_cast<int32>(g_scale))
    {
        m_playerTexturePosition.x -= DEFAULT_INTRO_MOVING_SPEED * g_scale;
        m_opponentTexturePosition.x += DEFAULT_INTRO_MOVING_SPEED * g_scale;
    }
    else
    {        
        m_finished = true;                     
    }
}

void FSMStateDarkIntro::render()
{    
    SDL_RenderCopy(g_pRenderer.get(), m_opponentDarkTexture->getTexture().get(), nullptr, &m_opponentTexturePosition);
    SDL_RenderCopy(g_pRenderer.get(), m_playerDarkTexture->getTexture().get(), nullptr, &m_playerTexturePosition);
}

std::unique_ptr<FSMState> FSMStateDarkIntro::getSuccessor() const
{
    return std::make_unique<FSMStateIntro>(
        m_normalTrainerAtlas,
        m_darkTrainerAtlas,
        m_activeComponents,
        m_localPokemon, 
        m_enemyPokemon, 
        m_isWildEncounter);
}