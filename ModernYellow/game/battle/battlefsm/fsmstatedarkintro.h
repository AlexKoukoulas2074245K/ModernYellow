/* ======================
   Date: 31/7/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once

#include "fsmstate.h"
#include "../battlecontroller.h"

#include <SDL_render.h>

class TextureResource;
class FSMStateDarkIntro: public FSMState
{
public:        
    FSMStateDarkIntro(
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
    std::shared_ptr<TextureResource> m_playerDarkTexture;
    std::shared_ptr<TextureResource> m_opponentDarkTexture;     
    SDL_Rect m_playerTexturePosition, m_opponentTexturePosition;
};