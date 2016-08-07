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
        const bool isWildEncounter);

    virtual ~FSMState() = 0;

    virtual void update() = 0;

    virtual void render() = 0;    

    virtual std::unique_ptr<FSMState> getSuccessor() const = 0;
    
    bool isFinished() const;

protected:

    void renderLocalPokemonStats(const std::shared_ptr<TextureResource>& resource);
    void renderLocalPokemonHpBar(const float percentDepleted);
    void renderLocalActor(const std::shared_ptr<TextureResource>& resource);
    void renderLocalPokemonName(const std::string& name);
    void renderLocalPokemonLevel(const int8 level);
    void renderLocalPokemonCurrentAndMaxHP(const int16 currHp, const int16 maxHp);
    void renderAllLocalPlayerSceneObjects(
        const std::shared_ptr<TextureResource>& pokemonStatsTexture,
        const std::shared_ptr<TextureResource>& actorTexture,
        const int8 pokemonLevel,
        const int16 currHp,
        const int16 maxHp,
        const std::string& pokemonName);
   
    void renderOpponentPokemonStats(const std::shared_ptr<TextureResource>& resource);
    void renderOpponentPokemonHpBar(const float percentDepleted);
    void renderOpponentActor(const std::shared_ptr<TextureResource>& resource);
    void renderOpponentPokemonName(const std::string& name);
    void renderOpponentPokemonLevel(const int8 level);
    void renderAllOpponentSceneObjects(
        const std::shared_ptr<TextureResource>& pokemonStatsTexture,
        const std::shared_ptr<TextureResource>& actorTexture,
        const int8 pokemonLevel,
        const int16 currHp,
        const int16 maxHp,
        const std::string& pokemonName);

protected:

    static const int32 DEFAULT_BLOCK_SIZE = 8;

    static const int32 LOCAL_POKEMON_STATS_X   = 72;
    static const int32 LOCAL_POKEMON_STATS_Y   = 64;
    static const int32 LOCAL_POKEMON_NAME_X    = 96;
    static const int32 LOCAL_POKEMON_NAME_Y    = 56;
    static const int32 LOCAL_POKEMON_LEVEL_X   = 120;
    static const int32 LOCAL_POKEMON_LEVEL_Y   = 64;
    static const int32 LOCAL_ACTOR_X           = 8;
    static const int32 LOCAL_ACTOR_Y           = 40;
    static const int32 LOCAL_POKEMON_CURR_HP_X = 104;
    static const int32 LOCAL_POKEMON_CURR_HP_Y = 80;
    static const int32 LOCAL_POKEMON_MAX_HP_X  = 136;
    static const int32 LOCAL_POKEMON_MAX_HP_Y  = 80;
    static const int32 LOCAL_POKEMON_HP_BAR_X  = 96;
    static const int32 LOCAL_POKEMON_HP_BAR_Y  = 75;

    static const int32 OPPONENT_POKEMON_STATS_X  = 8;
    static const int32 OPPONENT_POKEMON_STATS_Y  = 8;
    static const int32 OPPONENT_POKEMON_NAME_X   = 8;
    static const int32 OPPONENT_POKEMON_NAME_Y   = 0;
    static const int32 OPPONENT_POKEMON_LEVEL_X  = 40;
    static const int32 OPPONENT_POKEMON_LEVEL_Y  = 8;
    static const int32 OPPONENT_ACTOR_X          = 104;
    static const int32 OPPONENT_ACTOR_Y          = 0;
    static const int32 OPPONENT_POKEMON_HP_BAR_X = 32;
    static const int32 OPPONENT_POKEMON_HP_BAR_Y = 19;
    
    static const int32 POKEMON_BAR_WIDTH = 48;
    static const int32 POKEMON_BAR_HEIGHT = 2;

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