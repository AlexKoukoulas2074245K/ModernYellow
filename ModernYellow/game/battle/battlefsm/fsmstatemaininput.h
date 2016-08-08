/* ======================
   Date: 8/8/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once

#include "fsmstate.h"
#include "../battlecontroller.h"

class FSMStateMainInput: public FSMState 
{
	FSMStateMainInput(
		const std::shared_ptr<TextureResource> normalTrainerAtlas,
		const std::shared_ptr<TextureResource> darkTrainerAtlas,
		BattleController::uiComponentStack_t& uiComponents,
		const BattleController::pokemonParty_t& localPokemon,
		const BattleController::pokemonParty_t& enemyPokemon,
		const bool isWildEncounter);
	
	void update() override;

	void render() override;

	std::unique_ptr<FSMState> getSuccessor() const override;
};