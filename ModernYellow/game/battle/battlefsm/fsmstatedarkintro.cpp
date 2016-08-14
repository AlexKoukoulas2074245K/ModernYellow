/* ======================
   Date: 31/7/2016
   Author: Alex Koukoulas
   ====================== */

#include "fsmstatedarkintro.h"
#include "fsmstateintro.h"
#include "../../pokemon.h"

#include <SDL_log.h>

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

FSMStateDarkIntro::FSMStateDarkIntro(BattleController& battleController):
    
    FSMState(battleController)         
{
    m_playerDarkTexture = m_battleController.getDarkTrainerAtlas()->getSubTexture(
        PLAYER_PORTRAIT_TU, 
        PLAYER_PORTRAIT_TV, 
        TRAINER_PORTRAIT_WIDTH, 
        TRAINER_PORTRAIT_HEIGHT);        

    if (m_battleController.isWildBattle())    
        m_opponentDarkTexture = castResToTex(
            resmanager.loadResource("pkmnfront/" + m_battleController.getActiveEnemyPokemon().getName() + "_DARK.png", RT_TEXTURE));
    else
        m_opponentDarkTexture = m_battleController.getDarkTrainerAtlas()->getSubTexture(
            0,
            0,
            TRAINER_PORTRAIT_WIDTH, 
            TRAINER_PORTRAIT_HEIGHT);

    m_playerTexturePosition = 
	{
        static_cast<int32>(g_width), 
        static_cast<int32>(g_height - TRAINER_PORTRAIT_HEIGHT * g_scale - DEFAULT_TEXTBOX_HEIGHT * g_scale),
        static_cast<int32>(TRAINER_PORTRAIT_WIDTH * g_scale), 
        static_cast<int32>(TRAINER_PORTRAIT_HEIGHT * g_scale)
	};

    m_opponentTexturePosition = 
	{
        static_cast<int32>(-TRAINER_PORTRAIT_WIDTH * g_scale + 8 * g_scale), 
        0, 
        static_cast<int32>(TRAINER_PORTRAIT_WIDTH * g_scale), 
        static_cast<int32>(TRAINER_PORTRAIT_HEIGHT * g_scale)
	};
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
    return std::make_unique<FSMStateIntro>(m_battleController);
}