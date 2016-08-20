/* ======================
   Date: 8/8/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once

#include "fsmstate.h"
#include "../battlecontroller.h"


class FSMStateMainInput: public FSMState 
{
public:

	enum MainInputOption
	{
		MIO_FIGHT = 0,
		MIO_POKEMON = 1,
		MIO_ITEMS = 2,
		MIO_RUN = 3
	};
	
	FSMStateMainInput(BattleController& battleController);
	
	void update() override;

	void render() override;

	std::unique_ptr<FSMState> getSuccessor() const override;

private:

	void updateMainOptions();
	
	void onMoveSelectionFinish(const bool);

private:

	std::shared_ptr<TextureResource> m_mainInputTexture;
	MainInputOption m_currMainOption;
	int8 m_currMoveSelectedIndex;

};