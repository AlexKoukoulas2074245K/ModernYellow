/* ======================
   Date: 21/8/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once

#include "../../mydef.h"

class Pokemon;
class Move;
namespace battlecalc
{
	int8 calculateOpponentMoveIndex(
		const Pokemon& localPokemon, 
		const Pokemon& opponentPokemon, 
		const bool isWildBattle);

	int8 getNumberOfValidMoves(const std::array<std::unique_ptr<Move>, 4>& moves);

	bool isLocalPokemonOppening(
		const Pokemon& localPokemon,
		const Pokemon& opponentPokemon,
		const Move& localPokemonMove,
		const Move& opponentPokemonMove);

	int32 calculateDamage(
		const Pokemon& attackingPokemon, 
		const Pokemon& defendingPokemon, 
		const Move& attackingMove);

	bool calculateWhetherCriticalHit(
		const Pokemon& attackingPokemon,
		const Move& attackingMove);	
}
