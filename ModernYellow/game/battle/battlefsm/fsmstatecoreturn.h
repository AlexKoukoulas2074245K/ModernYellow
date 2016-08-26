/* ======================
   Date: 21/8/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once

#include "fsmstate.h"
#include "../battlecontroller.h"

class Pokemon;
class Move;
class ShakeGenerator;
class FSMStateCoreTurn: public FSMState
{
public:
	FSMStateCoreTurn(
		BattleController& battleController, 
		Pokemon& localPokemon, 
		Pokemon& opponentPokemon,
		Move& localPokemonMove,
		Move& opponentPokemonMove);

	void update() override;

	void render() override;

	std::unique_ptr<FSMState> getSuccessor() const override;

private:

	enum InnerState
	{
		IS_MOVE_USAGE,
		IS_MOVE_ANIMATION,
		IS_MOVE_SHAKE,
		IS_HP_REDUCTION
	};

	void showPokemonMoveUsageTextbox(const bool forLocalPokemon);

private:

	Pokemon&                        m_localPokemon;
	Pokemon&                        m_opponentPokemon;
	Move&                           m_localPokemonMove;
	Move&                           m_opponentPokemonMove;
	const bool                      m_localPokemonIsFaster;
	InnerState                      m_innerState;
	std::unique_ptr<ShakeGenerator> m_shakeGenerator;
};
