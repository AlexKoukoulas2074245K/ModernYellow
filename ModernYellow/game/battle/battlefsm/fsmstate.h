/* ======================
   Date: 31/7/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once
#include "../../../mydef.h"
#include "../battlecontroller.h"
#include "../../../resources/sresmanager.h"
#include "../../../resources/textureresource.h"

#include <vector>


class Pokemon;
class FSMState
{
public:

    FSMState(
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
        m_finished(false){};

    virtual ~FSMState() = 0 {};

    virtual void update() = 0;

    virtual void render() = 0;

    virtual std::unique_ptr<FSMState> getSuccessor() const = 0;
    
    bool isFinished() const { return m_finished; };

protected:

    static const int32 OPPONENT_POKEMON_STATS_X = 8;
    static const int32 OPPONENT_POKEMON_STATS_Y = 16;
    static const int32 LOCAL_POKEMON_STATS_X = 72;
    static const int32 LOCAL_POKEMON_STATS_Y = 72;
    static const int32 LOCAL_ACTOR_X = 8;
    static const int32 LOCAL_ACTOR_Y = 40;
    static const int32 REMOTE_ACTOR_X = 104;
    static const int32 REMOTE_ACTOR_Y = 0;

protected:

    std::shared_ptr<TextureResource> m_normalTrainerAtlas;
    std::shared_ptr<TextureResource> m_darkTrainerAtlas;
    std::shared_ptr<TextureResource> m_opponentPokemonStatsTexture;
    std::shared_ptr<TextureResource> m_localPokemonStatsTexture;

    const BattleController::pokemonParty_t& m_localPokemon;
    const BattleController::pokemonParty_t& m_enemyPokemon;
    
    std::stack<std::unique_ptr<UIComponent>>& m_activeComponents;
    
    bool m_isWildEncounter;
    bool m_finished;

};