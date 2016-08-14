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
class BattleController;
class FSMState
{
public:

    FSMState(BattleController& battleController);

    virtual ~FSMState() = 0;

    virtual void update() = 0;

    virtual void render() = 0;    

    virtual std::unique_ptr<FSMState> getSuccessor() const = 0;
    
    bool isFinished() const;

protected:

    void renderLocalPokemonStats();
    void renderLocalPokemonHpBar(const float percentDepleted);
    void renderLocalActor(const std::shared_ptr<TextureResource>& resource);
    void renderLocalPokemonName();
    void renderLocalPokemonLevel();
    void renderLocalPokemonCurrentAndMaxHP();
    void renderAllDefaultLocalSceneObjects();
   
    void renderOpponentPokemonStats();
    void renderOpponentPokemonHpBar(const float percentDepleted);
    void renderOpponentActor(const std::shared_ptr<TextureResource>& resource);
    void renderOpponentPokemonName();
    void renderOpponentPokemonLevel();
    void renderAllDefaultOpponentSceneObjects();

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

	BattleController& m_battleController;
    bool m_finished;

};