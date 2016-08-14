/* ======================
   Date: 8/8/2016
   Author: Alex Koukoulas
   ====================== */

#include "fsmstatemaininput.h"
#include "../../../font.h"
#include "../../../sinputhandler.h"

#include <unordered_map>
#include <array>

extern pRenderer_t g_pRenderer;
extern pFont_t g_pFont;
extern uint32 g_scale;
extern uint32 g_width;
extern uint32 g_height;

static const std::unordered_map<FSMStateMainInput::MainInputOption, std::array<int32, 2>> i_optionPositions = 
{
	{ FSMStateMainInput::MainInputOption::MIO_FIGHT, {72, 112} },
	{ FSMStateMainInput::MainInputOption::MIO_POKEMON, {120, 112} },
	{ FSMStateMainInput::MainInputOption::MIO_ITEMS, {72, 128} },
	{ FSMStateMainInput::MainInputOption::MIO_RUN, {120, 128} }
};

/* ==============
   Public Methods
   ============== */
FSMStateMainInput::FSMStateMainInput(BattleController& battleController)

	: FSMState(battleController)
	, m_mainInputTexture(castResToTex(resmanager.loadResource("misctex/battle_options.png", RT_TEXTURE)))
	, m_currMainOption(MainInputOption::MIO_FIGHT)
{

}
	
void FSMStateMainInput::update()
{
	if (m_battleController.getUIComponentStack().empty())
	{
		updateMainOptions();
	}
}

void FSMStateMainInput::render()
{
	renderAllDefaultLocalSceneObjects();
	renderAllDefaultOpponentSceneObjects();
	SDLRender(
		g_pRenderer, 
		m_mainInputTexture->getTexture().get(), 
		g_width - m_mainInputTexture->getScaledWidth(), 
		g_height - m_mainInputTexture->getScaledHeight(),
		m_mainInputTexture->getScaledWidth(),
		m_mainInputTexture->getScaledHeight());

	g_pFont->renderString(
		Font::FONT_RIGHT_ARROW_NORMAL, 
		i_optionPositions.at(m_currMainOption)[0] * static_cast<int32>(g_scale), 
		i_optionPositions.at(m_currMainOption)[1] * static_cast<int32>(g_scale));
}

std::unique_ptr<FSMState> FSMStateMainInput::getSuccessor() const
{
	return nullptr;
}

/* ===============
   Private Methods
   =============== */
void FSMStateMainInput::updateMainOptions()
{
	auto currOptionIndex = static_cast<int32>(m_currMainOption);
	if (ihandler.isKeyTapped(K_UP))
	{
		if (currOptionIndex > 1)
			currOptionIndex -= 2;
	}

	else if (ihandler.isKeyTapped(K_DOWN))
	{
		if (currOptionIndex < 2)
			currOptionIndex += 2;
	}
	else if (ihandler.isKeyTapped(K_RIGHT))
	{
		if (currOptionIndex % 2 == 0)
			++currOptionIndex;

	}
	else if (ihandler.isKeyTapped(K_LEFT))
	{
		if (currOptionIndex % 2 == 1)
			--currOptionIndex;
	}
	m_currMainOption = static_cast<MainInputOption>(currOptionIndex);
}