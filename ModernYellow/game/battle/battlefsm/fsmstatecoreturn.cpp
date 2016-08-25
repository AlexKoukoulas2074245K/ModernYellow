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
#include <SDL_timer.h>
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
	, m_innerState(IS_MOVE_USAGE)
{
	showPokemonMoveUsageTextbox(m_localPokemonIsFaster);
}

void FSMStateCoreTurn::update()
{
	switch (m_innerState)
	{
		case IS_MOVE_USAGE: 
		{
			
			m_battleController.getUIComponentStack().top()->update();
			if (m_battleController.getUIComponentStack().top()->isFinished())
				m_innerState = IS_MOVE_ANIMATION;
		} break;
	
		case IS_MOVE_ANIMATION:
		{
			m_innerState = IS_MOVE_SHAKE;
		} break;

		case IS_MOVE_SHAKE:
		{		
			setShakeOffset(x, 0);
			m_battleController.getUIComponentStack().top()->setShakeOffset(x, 0);
		} break;

		case IS_HP_REDUCTION:
		{
		} break;
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