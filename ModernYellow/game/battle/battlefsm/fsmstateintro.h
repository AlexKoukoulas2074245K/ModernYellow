/* ======================
   Date: 31/7/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once

#include "fsmstate.h"
#include "../battlecontroller.h"

#include <SDL_render.h>
#include <unordered_map>

class FSMStateIntro: public FSMState
{
public:
    FSMStateIntro(
        const std::shared_ptr<TextureResource> normalTrainerAtlas,
        const std::shared_ptr<TextureResource> darkTrainerAtlas,
        BattleController::uiComponentStack_t& uiComponents,
        const BattleController::pokemonParty_t& localPokemon,
        const BattleController::pokemonParty_t& enemyPokemon,
        const bool isWildEncounter);

    void update() override;

    void render() override;

    std::unique_ptr<FSMState> getSuccessor() const override;

private:

    enum InnerState
    {
        IS_TEXTBOX_DISPLAY,
        IS_DISPLAY_OPPONENT_POKEMON_STATUS,
        IS_PLAYER_RETREATING,
        IS_TEXTBOX_GO_POKEMON,
        IS_POKEMON_CHARGING
    };

    enum PokemonStateBallType
    {
        PSBT_READY,
        PSBT_STATUS,
        PSBT_FAINTED
    };

    std::shared_ptr<TextureResource> m_playerTexture;
    std::shared_ptr<TextureResource> m_opponentTexture;
    std::shared_ptr<TextureResource> m_playerPokemonBar;
    SDL_Rect m_playerTexturePosition, m_opponentTexturePosition;

    InnerState m_innerState;
    std::unordered_map<PokemonStateBallType, std::shared_ptr<TextureResource>> m_pokemonBarBallTextures;
};