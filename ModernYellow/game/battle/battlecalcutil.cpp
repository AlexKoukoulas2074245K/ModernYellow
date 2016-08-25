/* ======================
   Date: 21/8/2016
   Author: Alex Koukoulas
   ====================== */

#include "battlecalcutil.h"
#include "../pokemon.h"
#include "../move.h"
#include "../../gameinfo.h"
#include "../../jsonutil/json.h"

#include <random>
#include <ctime>
#include <array>

extern pGameInfo_t g_pGameInfo;

int8 battlecalc::calculateOpponentMoveIndex(
	const Pokemon& localPokemon,
	const Pokemon& opponentPokemon,
	const bool isWildBattle)
{
	std::srand(static_cast<uint32>(std::time(nullptr)));
	return std::rand() % battlecalc::getNumberOfValidMoves(localPokemon.getMoveSet());
}

int8 battlecalc::getNumberOfValidMoves(const std::array<std::unique_ptr<Move>, 4>& moves)
{
	for (int i = 0; i < 4; ++i)
	{
		if (!moves[i])
			return i;
	}

	return 4;
}

bool battlecalc::isLocalPokemonOppening(
	const Pokemon& localPokemon,
	const Pokemon& opponentPokemon,
	const Move& localPokemonMove,
	const Move& opponentPokemonMove)
{
	if (localPokemonMove.getMoveName() == "QUICK_ATTACK" &&
		opponentPokemonMove.getMoveName() != "QUICK_ATTACK")
		return true;
	else if (localPokemonMove.getMoveName() != "QUICK_ATTACK" &&
		opponentPokemonMove.getMoveName() == "QUICK_ATTACK")
		return false;

	return localPokemon.getStat(Pokemon::S_SPEED) > opponentPokemon.getStat(Pokemon::S_SPEED);
}

int32 battlecalc::calculateDamage(
	const Pokemon& attackingPokemon,
	const Pokemon& defendingPokemon,
	const Move& attackingMove)
{
	// Calculate Main Factor
	const auto levelFactor = (2 * attackingPokemon.getLevel() + 10)/250.0f;
	const auto attackDefenseFactor = Move::isTypeSpecial(attackingMove.getType()) ? 
		attackingPokemon.getStat(Pokemon::S_SPECIAL) / static_cast<float>(defendingPokemon.getStat(Pokemon::S_SPECIAL)):
		attackingPokemon.getStat(Pokemon::S_ATTACK)  / static_cast<float>(defendingPokemon.getStat(Pokemon::S_DEFENSE));
	const auto mainFactor = levelFactor * attackDefenseFactor * attackingMove.getPower() + 2;

	// Calculate Modifier
	std::srand(static_cast<uint32>(std::time(nullptr)));
	auto isStab = attackingMove.getType() == attackingPokemon.getType()[0];
	if (attackingPokemon.getType().size() == 2 && attackingPokemon.getType()[1] == attackingMove.getType())
		isStab = true;

	auto typeEff = g_pGameInfo->getTypeEffectiveness(attackingMove.getType())[defendingPokemon.getType()[0]].asFloat();
	if (defendingPokemon.getType().size() == 2)
		typeEff *= g_pGameInfo->getTypeEffectiveness(attackingMove.getType())[defendingPokemon.getType()[1]].asFloat();

	auto isCrit = calculateWhetherCriticalHit(attackingPokemon, attackingMove);
	const auto randVar = ((std::rand() % 16) + 85.0f) / 100.0f;
	const auto finalModifier = randVar * (isCrit ? 2.0f : 1.0f) * typeEff * (isStab ? 1.5f : 1.0f);

	return std::max(static_cast<int32>(mainFactor * finalModifier), 1);
}

bool battlecalc::calculateWhetherCriticalHit(
	const Pokemon& attackingPokemon,
	const Move& attackingMove)
{
	std::srand(static_cast<uint32>(std::time(nullptr)));

	if (Move::isMoveHighCrit(attackingMove.getMoveName()))
	{
		return (std::rand() % 100) <= (attackingPokemon.getBaseStat(Pokemon::S_SPEED) * 100) / 64;
	}
	else
	{
		return (std::rand() % 100) <= (attackingPokemon.getBaseStat(Pokemon::S_SPEED) * 100) / 512;
	}
}