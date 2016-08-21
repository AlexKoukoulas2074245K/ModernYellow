/* ======================
   Date: 21/8/2016
   Author: Alex Koukoulas
   ====================== */

#include "fsmstatecoreturn.h"
#include "../../pokemon.h"
#include "../../move.h"
#include "../battlecalcutil.h"
#include "../../uicomps/uitextbox.h"

#include <SDL_log.h>

/* ==============
   Public Methods
   ============== */
FSMStateCoreTurn::FSMStateCoreTurn(
	BattleController& battleController, 
	Pokemon& localPokemon,
	Pokemon& opponentPokemon,
	Move& localPokemonMove,
	Move& opponentPokemonMove)

	: FSMState(battleController)
	, m_localPokemon(localPokemon)
	, m_opponentPokemon(opponentPokemon)
	, m_localPokemonMove(localPokemonMove)
	, m_opponentPokemonMove(opponentPokemonMove)
	, m_localPokemonIsFaster(battlecalc::isLocalPokemonOppening(
		localPokemon, 
		opponentPokemon, 
		localPokemonMove, 
		opponentPokemonMove))
{
	showPokemonMoveUsageTextbox(m_localPokemonIsFaster);
}

void FSMStateCoreTurn::update()
{
	if (!m_battleController.getUIComponentStack().top()->isFinished())
	{
		m_battleController.getUIComponentStack().top()->update();
		if (m_battleController.getUIComponentStack().top()->isFinished())
		{
			SDL_Log(std::to_string(battlecalc::calculateDamage(m_localPokemon, m_opponentPokemon, m_localPokemonMove)).c_str());
		}
	}
}

void FSMStateCoreTurn::render()
{
	renderAllDefaultLocalSceneObjects();
	renderAllDefaultOpponentSceneObjects();
	m_battleController.getUIComponentStack().top()->render();
}

std::unique_ptr<FSMState> FSMStateCoreTurn::getSuccessor() const
{
	return nullptr;
}

/* ==============
   Public Methods
   ============== */
void FSMStateCoreTurn::showPokemonMoveUsageTextbox(const bool forLocalPokemon)
{
	if (forLocalPokemon)
		m_battleController.getUIComponentStack().push(std::make_unique<UITextbox>(m_localPokemon.getName() + "#used " + m_localPokemonMove.getMoveName() + "!^"));
	else
		m_battleController.getUIComponentStack().push(std::make_unique<UITextbox>("Enemy " + m_opponentPokemon.getName() + "#used " + m_opponentPokemonMove.getMoveName() + "!^"));
}