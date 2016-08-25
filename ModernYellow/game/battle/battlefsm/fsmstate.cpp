/* ======================
   Date: 3/8/2016
   Author: Alex Koukoulas
   ====================== */

#include "fsmstate.h"
#include "../battlecontroller.h"
#include "../../pokemon.h"
#include "../../../font.h"
#include <SDL_log.h>

extern pRenderer_t g_pRenderer;
extern pFont_t g_pFont;
extern uint32 g_scale;

/* ==============
   Public Methods
   ============== */

FSMState::FSMState(BattleController& battleController)

	: m_battleController(battleController)
    , m_finished(false)
	, m_shakeXoffset(0)
	, m_shakeYoffset(0)
{
}

FSMState::~FSMState(){}

bool FSMState::isFinished() const 
{
    return m_finished;
}

int32 FSMState::getShakeXoffset() const
{
	return m_shakeXoffset;
}

int32 FSMState::getShakeYoffset() const
{
	return m_shakeYoffset;
}

/* =================
   Protected Methods
   ================= */

// Local Player helper rendering methods
void FSMState::renderLocalPokemonStats()
{
    SDLRender(
        g_pRenderer,
        m_battleController.getLocalPokemonStatsTexture()->getTexture().get(),
        LOCAL_POKEMON_STATS_X * g_scale + m_shakeXoffset * g_scale,
        LOCAL_POKEMON_STATS_Y * g_scale + m_shakeYoffset * g_scale,                
		m_battleController.getLocalPokemonStatsTexture()->getScaledWidth(),
		m_battleController.getLocalPokemonStatsTexture()->getScaledHeight());
}

void FSMState::renderLocalPokemonHpBar(const float percentDepleted)
{
    const float remainingPercent = 1.0f - percentDepleted;
    uint32 barColor = envcolors::BC_GREEN;
        
    if (remainingPercent < 0.25f)
        barColor = envcolors::BC_CRITI;
    else if (remainingPercent < 0.5f)
        barColor = envcolors::BC_ORANG;

    SDL_SetRenderDrawColor(
        g_pRenderer.get(), 
        RED(barColor),
        GREEN(barColor), 
        BLUE(barColor),
        ALPHA(barColor));

    SDL_Rect hpArea = 
    { 
        static_cast<int32>(LOCAL_POKEMON_HP_BAR_X * g_scale + m_shakeXoffset * g_scale), 
        static_cast<int32>(LOCAL_POKEMON_HP_BAR_Y * g_scale + m_shakeYoffset * g_scale),
        static_cast<int32>((remainingPercent * POKEMON_BAR_WIDTH) * g_scale),
        static_cast<int32>(POKEMON_BAR_HEIGHT * g_scale)
    };

    SDL_RenderFillRect(g_pRenderer.get(), &hpArea);
}

void FSMState::renderLocalActor(const std::shared_ptr<TextureResource>& resource)
{
    SDLRender(
        g_pRenderer,
        resource->getTexture().get(),
        LOCAL_ACTOR_X * g_scale + m_shakeXoffset * g_scale,
        LOCAL_ACTOR_Y * g_scale + m_shakeYoffset * g_scale,
        resource->getScaledWidth(),
        resource->getScaledHeight());
}

void FSMState::renderLocalPokemonName()
{
    g_pFont->renderString(
		m_battleController.getActiveLocalPokemon().getName(), 
		LOCAL_POKEMON_NAME_X * g_scale + m_shakeXoffset * g_scale, 
		LOCAL_POKEMON_NAME_Y * g_scale + m_shakeYoffset * g_scale);
}

void FSMState::renderLocalPokemonLevel()
{
    g_pFont->renderString(
		std::to_string(m_battleController.getActiveLocalPokemon().getLevel()),
		LOCAL_POKEMON_LEVEL_X * g_scale + m_shakeXoffset * g_scale, 
		LOCAL_POKEMON_LEVEL_Y * g_scale + m_shakeYoffset * g_scale);
}

void FSMState::renderLocalPokemonCurrentAndMaxHP()
{
    const auto currHpString = std::to_string(m_battleController.getActiveLocalPokemon().getCurrHp());
    auto lastCharIndex = currHpString.size() - 1;
    for (int32 i = lastCharIndex; i >= 0; --i)
        g_pFont->renderString(
            std::string(1, currHpString[i]),
            LOCAL_POKEMON_CURR_HP_X * g_scale - (lastCharIndex - i) * DEFAULT_BLOCK_SIZE * g_scale + m_shakeXoffset * g_scale,
            LOCAL_POKEMON_CURR_HP_Y * g_scale + m_shakeYoffset * g_scale);
    
    const auto maxHpString = std::to_string(m_battleController.getActiveLocalPokemon().getStat(Pokemon::S_HP));
    lastCharIndex = maxHpString.size() - 1;
    for (int32 i = lastCharIndex; i >= 0; --i)
        g_pFont->renderString(
            std::string(1, maxHpString[i]),
            LOCAL_POKEMON_MAX_HP_X * g_scale - (lastCharIndex - i) * DEFAULT_BLOCK_SIZE * g_scale + m_shakeXoffset * g_scale,
            LOCAL_POKEMON_MAX_HP_Y * g_scale + m_shakeYoffset * g_scale);
}

void FSMState::renderAllDefaultLocalSceneObjects()
{
    renderLocalPokemonStats();
    renderLocalPokemonHpBar(1.0f - static_cast<float>(m_battleController.getActiveLocalPokemon().getCurrHp()) / static_cast<float>(m_battleController.getActiveLocalPokemon().getStat(Pokemon::S_HP)));
    renderLocalActor(m_battleController.getLocalPokemonActorTexture(m_battleController.getActiveLocalPokemon().getName()));
    renderLocalPokemonLevel();
    renderLocalPokemonName();
    renderLocalPokemonCurrentAndMaxHP();
}

// Opponent player helper rendering methods
void FSMState::renderOpponentPokemonStats()
{
    SDLRender(
        g_pRenderer,
        m_battleController.getEnemyPokemonStatsTexture()->getTexture().get(),
        OPPONENT_POKEMON_STATS_X * g_scale + m_shakeXoffset * g_scale,
        OPPONENT_POKEMON_STATS_Y * g_scale + m_shakeYoffset * g_scale,
        m_battleController.getEnemyPokemonStatsTexture()->getScaledWidth(),
        m_battleController.getEnemyPokemonStatsTexture()->getScaledHeight());
}

void FSMState::renderOpponentPokemonHpBar(const float percentDepleted)
{
    const float remainingPercent = 1.0f - percentDepleted;
    uint32 barColor = envcolors::BC_GREEN;
    
    if (remainingPercent < 0.25f)
        barColor = envcolors::BC_CRITI;
    else if (remainingPercent < 0.5f)
        barColor = envcolors::BC_ORANG;

    SDL_SetRenderDrawColor(
        g_pRenderer.get(),
        RED(barColor),
        GREEN(barColor),
        BLUE(barColor),
        ALPHA(barColor));

    SDL_Rect hpArea =
    {
        static_cast<int32>(OPPONENT_POKEMON_HP_BAR_X * g_scale + m_shakeXoffset * g_scale),
        static_cast<int32>(OPPONENT_POKEMON_HP_BAR_Y * g_scale + m_shakeYoffset * g_scale),
        static_cast<int32>((remainingPercent * POKEMON_BAR_WIDTH) * g_scale),
        static_cast<int32>(POKEMON_BAR_HEIGHT * g_scale)
    };

    SDL_RenderFillRect(g_pRenderer.get(), &hpArea);
}

void FSMState::renderOpponentPokemonName()
{
    g_pFont->renderString(
		m_battleController.getActiveEnemyPokemon().getName(),
		OPPONENT_POKEMON_NAME_X * g_scale + m_shakeXoffset * g_scale, 
		OPPONENT_POKEMON_NAME_Y * g_scale + m_shakeYoffset * g_scale);
}

void FSMState::renderOpponentPokemonLevel()
{
    g_pFont->renderString(
		std::to_string(m_battleController.getActiveEnemyPokemon().getLevel()), 
		OPPONENT_POKEMON_LEVEL_X * g_scale + m_shakeXoffset * g_scale,
		OPPONENT_POKEMON_LEVEL_Y * g_scale + m_shakeYoffset * g_scale);
}

void FSMState::renderOpponentActor(const std::shared_ptr<TextureResource>& resource)
{
    SDLRender(
        g_pRenderer,
        resource->getTexture().get(),
        OPPONENT_ACTOR_X * g_scale + m_shakeXoffset * g_scale,
        OPPONENT_ACTOR_Y * g_scale + m_shakeYoffset * g_scale,
        resource->getScaledWidth(),
        resource->getScaledHeight());
}

void FSMState::renderAllDefaultOpponentSceneObjects()
{
    renderOpponentPokemonStats();    
    renderOpponentPokemonHpBar(1.0f - static_cast<float>(m_battleController.getActiveEnemyPokemon().getCurrHp()) / static_cast<float>(m_battleController.getActiveEnemyPokemon().getStat(Pokemon::S_HP)));
	renderOpponentActor(m_battleController.getEnemyPokemonActorTexture(m_battleController.getActiveEnemyPokemon().getName()));
    renderOpponentPokemonLevel();
    renderOpponentPokemonName();
}

void FSMState::setShakeOffset(const int32 xOffset, const int32 yOffset)
{
	m_shakeXoffset = xOffset;
	m_shakeYoffset = yOffset;
}