/* ======================
   Date: 31/7/2016
   Author: Alex Koukoulas
   ====================== */

#include "fsmstateintro.h"
#include "fsmstatemaininput.h"
#include "../battlecontroller.h"
#include "../../uicomps/uitextbox.h"
#include "../../../mixer.h"
#include "../../../font.h"
#include "../../pokemon.h"
#include "../../../sinputhandler.h"

#include <SDL_log.h>
#include <SDL_timer.h>

extern pMixer_t    g_pMixer;
extern pRenderer_t g_pRenderer;
extern pFont_t     g_pFont;
extern uint32      g_scale;
extern uint32      g_width;
extern uint32      g_height;

/* ==================
   Internal Constants
   ================== */
static const int32 PLAYER_POKEMON_BAR_X = 72;
static const int32 PLAYER_POKEMON_BAR_Y = 80;
static const int32 PLAYER_POKEMON_BAR_FIRST_BALL_X = 88;
static const int32 PLAYER_POKEMON_BAR_FIRST_BALL_Y = 80;
static const int32 PLAYER_AND_POKEMON_MOVE_SPEED = 4;

/* ==============
   Public Methods
   ============== */
FSMStateIntro::FSMStateIntro(BattleController& battleController):

    FSMState(battleController),
    m_innerState(IS_TEXTBOX_DISPLAY)
{    
    if (m_battleController.isWildBattle())
        g_pMixer->playAudio("sfx/cries/" + m_battleController.getActiveEnemyPokemon().getID() + ".wav", false, true);            

    // Get player trainer texture
    m_playerTexture = m_battleController.getNormalTrainerAtlas()->getSubTexture(
        PLAYER_PORTRAIT_TU,
        PLAYER_PORTRAIT_TV,
        TRAINER_PORTRAIT_WIDTH,
        TRAINER_PORTRAIT_HEIGHT);

    // Load respective opponent texture
    if (m_battleController.isWildBattle())
        m_opponentTexture = castResToTex(
            resmanager.loadResource("pkmnfront/" + m_battleController.getActiveEnemyPokemon().getName() + ".png", RT_TEXTURE));
    else
        m_opponentTexture = m_battleController.getNormalTrainerAtlas()->getSubTexture(
            0,
            0,
            TRAINER_PORTRAIT_WIDTH,
            TRAINER_PORTRAIT_HEIGHT);

    // Calculate player and opponent/pokemon texture positions
    m_playerTexturePosition = 
	{
        static_cast<int32>(LOCAL_ACTOR_X * g_scale),
        static_cast<int32>(LOCAL_ACTOR_Y * g_scale),
        static_cast<int32>(TRAINER_PORTRAIT_WIDTH * g_scale),
        static_cast<int32>(TRAINER_PORTRAIT_HEIGHT * g_scale)
	};

    m_opponentTexturePosition = 
	{
		static_cast<int32>(OPPONENT_ACTOR_X * g_scale),
		static_cast<int32>(OPPONENT_ACTOR_Y * g_scale),
		static_cast<int32>(TRAINER_PORTRAIT_WIDTH * g_scale),
		static_cast<int32>(TRAINER_PORTRAIT_HEIGHT * g_scale) 
	};
	
    // Load player pokemon bar
    m_playerPokemonBar = castResToTex(resmanager.loadResource("misctex/player_pokemon.png", RT_TEXTURE));

    // Load player pokemon bar ball textures
    m_pokemonBarBallTextures[PSBT_READY]   = castResToTex(resmanager.loadResource("misctex/ballBattle.png", RT_TEXTURE));
    m_pokemonBarBallTextures[PSBT_STATUS]  = castResToTex(resmanager.loadResource("misctex/ballStatus.png", RT_TEXTURE));
    m_pokemonBarBallTextures[PSBT_FAINTED] = castResToTex(resmanager.loadResource("misctex/ballFainted.png", RT_TEXTURE));

    // Create textbox for the respective battle type
    if (m_battleController.isWildBattle())
    {
        m_battleController.getUIComponentStack().push(std::make_unique<UITextbox>("A wild " + m_battleController.getActiveEnemyPokemon().getName() + "#appeared!*"));
    }
}

void FSMStateIntro::update()
{
    switch (m_innerState)
    {
        case IS_TEXTBOX_DISPLAY:
        {
            if (!m_battleController.getUIComponentStack().top()->isFinished())
				m_battleController.getUIComponentStack().top()->update();
            else
            {
                m_innerState = IS_DISPLAY_OPPONENT_POKEMON_STATUS;
				m_battleController.getUIComponentStack().pop();
            }
        } break;

        case IS_DISPLAY_OPPONENT_POKEMON_STATUS:
        {            
            SDL_Delay(500);
            m_innerState = IS_PLAYER_RETREATING;
        } break;
        
        case IS_PLAYER_RETREATING:
        {
            m_playerTexturePosition.x -= PLAYER_AND_POKEMON_MOVE_SPEED * g_scale;
            if (m_playerTexturePosition.x <= -(static_cast<int32>(m_playerTexture->getScaledWidth())))
            {
                m_innerState = IS_TEXTBOX_GO_POKEMON;
				m_battleController.getUIComponentStack().push(std::make_unique<UITextbox>("Go! " + m_battleController.getActiveLocalPokemon().getName() + "!^"));
            }
        } break;

        case IS_TEXTBOX_GO_POKEMON:
        {
            if (!m_battleController.getUIComponentStack().top()->isFinished())
            {
				m_battleController.getUIComponentStack().top()->update();
            }
            else
            {
                SDL_Delay(100);
                m_innerState = IS_POKEMON_CHARGING;
                m_playerTexture = castResToTex(resmanager.loadResource("pkmnback/" + m_battleController.getActiveLocalPokemon().getName() + ".png", RT_TEXTURE));
            }
        } break;

        case IS_POKEMON_CHARGING:
        {
            if (m_playerTexturePosition.x >= LOCAL_ACTOR_X * static_cast<int32>(g_scale))
            {
                m_playerTexturePosition.x = LOCAL_ACTOR_X * static_cast<int32>(g_scale);
                g_pMixer->playAudio("sfx/cries/" + m_battleController.getActiveLocalPokemon().getID() + ".wav", false, false);                
                SDL_Delay(1000);
				m_battleController.getUIComponentStack().pop();
                m_finished = true;
            }
            m_playerTexturePosition.x += PLAYER_AND_POKEMON_MOVE_SPEED * g_scale;
        } break;
    }

    
}

void FSMStateIntro::render()
{
    switch (m_innerState)
    {
        case IS_TEXTBOX_DISPLAY:
        {
            SDL_RenderCopy(g_pRenderer.get(), m_opponentTexture->getTexture().get(), nullptr, &m_opponentTexturePosition);
            SDL_RenderCopy(g_pRenderer.get(), m_playerTexture->getTexture().get(), nullptr, &m_playerTexturePosition);            

            SDLRender(
                g_pRenderer,
                m_playerPokemonBar->getTexture().get(),
                PLAYER_POKEMON_BAR_X * g_scale,
                PLAYER_POKEMON_BAR_Y * g_scale,
                m_playerPokemonBar->getScaledWidth(),
                m_playerPokemonBar->getScaledHeight());

            auto ballPositionIter = PLAYER_POKEMON_BAR_FIRST_BALL_X * g_scale;
            for (const auto& pokemon : m_battleController.getLocalPokemonParty())
            {
                auto pokemonState = PSBT_READY;

                if (pokemon->hasFainted())
                    pokemonState = PSBT_FAINTED;
                else if (pokemon->getStatus() != Pokemon::Status::STATUS_OK)
                    pokemonState = PSBT_STATUS;

                SDLRender(
                    g_pRenderer,
                    m_pokemonBarBallTextures[pokemonState]->getTexture().get(),
                    ballPositionIter,
                    PLAYER_POKEMON_BAR_FIRST_BALL_Y * g_scale,
                    m_pokemonBarBallTextures[pokemonState]->getScaledWidth(),
                    m_pokemonBarBallTextures[pokemonState]->getScaledHeight());

                ballPositionIter += DEFAULT_BLOCK_SIZE * g_scale;
            }
            
			m_battleController.getUIComponentStack().top()->render();
        } break;

        case IS_PLAYER_RETREATING:
        case IS_DISPLAY_OPPONENT_POKEMON_STATUS:
        {
            SDL_RenderCopy(g_pRenderer.get(), m_opponentTexture->getTexture().get(), nullptr, &m_opponentTexturePosition);
            SDL_RenderCopy(g_pRenderer.get(), m_playerTexture->getTexture().get(), nullptr, &m_playerTexturePosition);

            renderOpponentPokemonStats();
            renderOpponentPokemonHpBar(1.0f - static_cast<float>(m_battleController.getActiveEnemyPokemon().getCurrHp()) / static_cast<float>(m_battleController.getActiveEnemyPokemon().getStat(Pokemon::S_HP)));
            renderOpponentPokemonName();
            renderOpponentPokemonLevel();

        } break;

        case IS_TEXTBOX_GO_POKEMON:
        {
            SDL_RenderCopy(g_pRenderer.get(), m_opponentTexture->getTexture().get(), nullptr, &m_opponentTexturePosition);
            SDL_RenderCopy(g_pRenderer.get(), m_playerTexture->getTexture().get(), nullptr, &m_playerTexturePosition);

            renderOpponentPokemonStats();
            renderOpponentPokemonHpBar(1.0f - static_cast<float>(m_battleController.getActiveEnemyPokemon().getCurrHp()) / static_cast<float>(m_battleController.getActiveEnemyPokemon().getStat(Pokemon::S_HP)));
            renderOpponentPokemonName();
            renderOpponentPokemonLevel();

			m_battleController.getUIComponentStack().top()->render();
        } break;

        case IS_POKEMON_CHARGING:
        {
            SDL_RenderCopy(g_pRenderer.get(), m_opponentTexture->getTexture().get(), nullptr, &m_opponentTexturePosition);
            SDL_RenderCopy(g_pRenderer.get(), m_playerTexture->getTexture().get(), nullptr, &m_playerTexturePosition);
            
            renderLocalPokemonStats();
            renderLocalPokemonHpBar(1.0f - static_cast<float>(m_battleController.getActiveLocalPokemon().getCurrHp()) / static_cast<float>(m_battleController.getActiveLocalPokemon().getStat(Pokemon::S_HP)));
            renderLocalPokemonName();
            renderLocalPokemonLevel();
            renderLocalPokemonCurrentAndMaxHP();
			
            renderOpponentPokemonStats();
            renderOpponentPokemonHpBar(1.0f - static_cast<float>(m_battleController.getActiveEnemyPokemon().getCurrHp()) / static_cast<float>(m_battleController.getActiveEnemyPokemon().getStat(Pokemon::S_HP)));
            renderOpponentPokemonName();
            renderOpponentPokemonLevel();
            
			m_battleController.getUIComponentStack().top()->render();
            
        } break;
    }
    
}

std::unique_ptr<FSMState> FSMStateIntro::getSuccessor() const
{
    return std::make_unique<FSMStateMainInput>(m_battleController);
}