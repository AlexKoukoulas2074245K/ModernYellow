/* ======================
   Date: 8/8/2016
   Author: Alex Koukoulas
   ====================== */

#include "fsmstatemaininput.h"

FSMStateMainInput::FSMStateMainInput(
	const std::shared_ptr<TextureResource> normalTrainerAtlas,
	const std::shared_ptr<TextureResource> darkTrainerAtlas,
	BattleController::uiComponentStack_t& uiComponents,
	const BattleController::pokemonParty_t& localPokemon,
	const BattleController::pokemonParty_t& enemyPokemon,
	const bool isWildEncounter) :

	FSMState(
		normalTrainerAtlas,
		darkTrainerAtlas,
		uiComponents,
		localPokemon,
		enemyPokemon,
		isWildEncounter)
{

}
	
void FSMStateMainInput::update()
{

}

void FSMStateMainInput::render()
{
	
}

std::unique_ptr<FSMState> FSMStateMainInput::getSuccessor() const
{

}