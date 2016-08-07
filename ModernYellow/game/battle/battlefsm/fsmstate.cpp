/* ======================
   Date: 3/8/2016
   Author: Alex Koukoulas
   ====================== */

#include "fsmstate.h"
#include "../../../font.h"
#include <SDL_log.h>

extern pRenderer_t g_pRenderer;
extern pFont_t g_pFont;
extern uint32 g_scale;

/* ==============
   Public Methods
   ============== */

FSMState::FSMState(
    std::shared_ptr<TextureResource> normalTrainerAtlas,
    std::shared_ptr<TextureResource> darkTrainerAtlas,
    BattleController::uiComponentStack_t& uiComponents,
    const BattleController::pokemonParty_t& localPokemon,
    const BattleController::pokemonParty_t& enemyPokemon,
    const bool isWildEncounter):

    m_normalTrainerAtlas(normalTrainerAtlas),
    m_darkTrainerAtlas(darkTrainerAtlas),
    m_opponentPokemonStatsTexture(castResToTex(resmanager.loadResource("misctex/enemy_stats.png", RT_TEXTURE))),
    m_localPokemonStatsTexture(castResToTex(resmanager.loadResource("misctex/player_stats.png", RT_TEXTURE))),
    m_activeComponents(uiComponents),
    m_localPokemon(localPokemon),
    m_enemyPokemon(enemyPokemon),
    m_isWildEncounter(isWildEncounter),
    m_finished(false){}

FSMState::~FSMState(){}

bool FSMState::isFinished() const 
{
    return m_finished;
}

/* =================
   Protected Methods
   ================= */

// Local Player helper rendering methods
void FSMState::renderLocalPokemonStats(const std::shared_ptr<TextureResource>& resource)
{
    SDLRender(
        g_pRenderer,
        resource->getTexture().get(),
        LOCAL_POKEMON_STATS_X * g_scale,
        LOCAL_POKEMON_STATS_Y * g_scale,                
        resource->getScaledWidth(),
        resource->getScaledHeight());
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
        LOCAL_POKEMON_HP_BAR_X * g_scale, 
        LOCAL_POKEMON_HP_BAR_Y * g_scale,
        static_cast<int32>((remainingPercent * POKEMON_BAR_WIDTH) * g_scale),
        POKEMON_BAR_HEIGHT * g_scale
    };

    SDL_RenderFillRect(g_pRenderer.get(), &hpArea);
}

void FSMState::renderLocalActor(const std::shared_ptr<TextureResource>& resource)
{
    SDLRender(
        g_pRenderer,
        resource->getTexture().get(),
        LOCAL_ACTOR_X * g_scale,
        LOCAL_ACTOR_Y * g_scale,
        resource->getScaledWidth(),
        resource->getScaledHeight());
}

void FSMState::renderLocalPokemonName(const std::string& name)
{
    g_pFont->renderString(name, LOCAL_POKEMON_NAME_X * g_scale, LOCAL_POKEMON_NAME_Y * g_scale);
}

void FSMState::renderLocalPokemonLevel(const int8 level)
{
    g_pFont->renderString(std::to_string(level), LOCAL_POKEMON_LEVEL_X * g_scale, LOCAL_POKEMON_LEVEL_Y * g_scale);
}

void FSMState::renderLocalPokemonCurrentAndMaxHP(const int16 currHp, const int16 maxHp)
{
    const auto currHpString = std::to_string(currHp);
    auto lastCharIndex = currHpString.size() - 1;
    for (int32 i = lastCharIndex; i >= 0; --i)
        g_pFont->renderString(
            std::string(1, currHpString[i]),
            LOCAL_POKEMON_CURR_HP_X * g_scale - (lastCharIndex - i) * DEFAULT_BLOCK_SIZE * g_scale,
            LOCAL_POKEMON_CURR_HP_Y * g_scale);
    
    const auto maxHpString = std::to_string(maxHp);
    lastCharIndex = maxHpString.size() - 1;
    for (int32 i = lastCharIndex; i >= 0; --i)
        g_pFont->renderString(
            std::string(1, maxHpString[i]),
            LOCAL_POKEMON_MAX_HP_X * g_scale - (lastCharIndex - i) * DEFAULT_BLOCK_SIZE * g_scale,
            LOCAL_POKEMON_MAX_HP_Y * g_scale);
}

void FSMState::renderAllLocalPlayerSceneObjects(
    const std::shared_ptr<TextureResource>& pokemonStatsTexture,
    const std::shared_ptr<TextureResource>& actorTexture,
    const int8 pokemonLevel,
    const int16 currHp,
    const int16 maxHp,    
    const std::string& pokemonName)
{
    renderLocalPokemonStats(pokemonStatsTexture);
    renderLocalPokemonHpBar(1.0f - static_cast<float>(currHp) / static_cast<float>(maxHp));
    renderLocalActor(actorTexture);
    renderLocalPokemonLevel(pokemonLevel);
    renderLocalPokemonName(pokemonName);
    renderLocalPokemonCurrentAndMaxHP(currHp, maxHp);
}

// Opponent player helper rendering methods
void FSMState::renderOpponentPokemonStats(const std::shared_ptr<TextureResource>& resource)
{
    SDLRender(
        g_pRenderer,
        resource->getTexture().get(),
        OPPONENT_POKEMON_STATS_X * g_scale,
        OPPONENT_POKEMON_STATS_Y * g_scale,
        resource->getScaledWidth(),
        resource->getScaledHeight());
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
        OPPONENT_POKEMON_HP_BAR_X * g_scale,
        OPPONENT_POKEMON_HP_BAR_Y * g_scale,
        static_cast<int32>((remainingPercent * POKEMON_BAR_WIDTH) * g_scale),
        POKEMON_BAR_HEIGHT * g_scale
    };

    SDL_RenderFillRect(g_pRenderer.get(), &hpArea);
}

void FSMState::renderOpponentPokemonName(const std::string& name)
{
    g_pFont->renderString(name, OPPONENT_POKEMON_NAME_X * g_scale, OPPONENT_POKEMON_NAME_Y * g_scale);
}

void FSMState::renderOpponentPokemonLevel(const int8 level)
{
    g_pFont->renderString(std::to_string(level), OPPONENT_POKEMON_LEVEL_X * g_scale, OPPONENT_POKEMON_LEVEL_Y * g_scale);
}

void FSMState::renderOpponentActor(const std::shared_ptr<TextureResource>& resource)
{
    SDLRender(
        g_pRenderer,
        resource->getTexture().get(),
        OPPONENT_ACTOR_X * g_scale,
        OPPONENT_ACTOR_Y * g_scale,
        resource->getScaledWidth(),
        resource->getScaledHeight());
}

void FSMState::renderAllOpponentSceneObjects(
    const std::shared_ptr<TextureResource>& pokemonStatsTexture,
    const std::shared_ptr<TextureResource>& actorTexture,
    const int8 pokemonLevel,    
    const int16 currHp,
    const int16 maxHp,
    const std::string& pokemonName)
{
    renderOpponentPokemonStats(pokemonStatsTexture);    
    renderOpponentPokemonHpBar(1.0f - static_cast<float>(currHp) / static_cast<float>(maxHp));
    renderOpponentActor(actorTexture);
    renderOpponentPokemonLevel(pokemonLevel);
    renderOpponentPokemonName(pokemonName);
}