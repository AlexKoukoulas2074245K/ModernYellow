/* ======================
   Date: 20/8/2016
   Author: Alex Koukoulas
   ====================== */

#include "uimoveselect.h"
#include "../../resources/textureresource.h"
#include "../../resources/sresmanager.h"
#include "../move.h"
#include "../../font.h"
#include "../../sinputhandler.h"
#include "../../mixer.h"

extern pFont_t     g_pFont;
extern pRenderer_t g_pRenderer;
extern pMixer_t    g_pMixer;
extern uint32      g_scale;

static const int32 MOVE_SELECT_BASE_X     = 0;
static const int32 MOVE_SELECT_BASE_Y     = 64;
static const int32 MOVE_ACTIVE_TYPE_X     = 16;
static const int32 MOVE_ACTIVE_TYPE_Y     = 80;
static const int32 MOVE_ACTIVE_CURR_PP_X  = 48;
static const int32 MOVE_ACTIVE_MAX_PP_X   = 72;
static const int32 MOVE_PP_Y              = 88;
static const int32 MOVE_FIRST_MOVE_X      = 48;
static const int32 MOVE_FIRST_MOVE_Y      = 104;
static const int32 MOVE_SELECTION_ARROW_X = 40;
static const int32 MOVE_SELECTION_ARROW_Y = 104;

/* ==================
   Internal Functions
   ================== */
static int8 getNumberOfValidMoves(const std::array<std::unique_ptr<Move>, 4>& moves);

/* ==============
   Public Methods
   ============== */
UIMoveSelect::UIMoveSelect(
	const std::array<std::unique_ptr<Move>, 4>& moves,
	const std::array<int8, 4>& pps,
	const std::function<void(const bool)> onCompleteCallback,
	int8& outMoveSelectedIndex)

	: UIComponent("misctex/moveselection.png", MOVE_SELECT_BASE_X * g_scale, MOVE_SELECT_BASE_Y * g_scale)
	, m_noMoveTex(castResToTex(resmanager.loadResource("misctex/nomove.png", RT_TEXTURE)))
	, m_moves(moves)
	, m_pps(pps)
	, m_selectedMove(outMoveSelectedIndex)
	, m_onCompleteCallback(onCompleteCallback)
	, m_pressedBack(false)
	, m_numValidMoves(getNumberOfValidMoves(moves))
{
	
}

void UIMoveSelect::update()
{
	if (ihandler.isKeyTapped(K_UP))
	{
		m_selectedMove--;
		if (m_selectedMove < 0)
			m_selectedMove = m_numValidMoves - 1;
	}
	else if (ihandler.isKeyTapped(K_DOWN))
	{
		m_selectedMove = (m_selectedMove + 1) % m_numValidMoves;
	}
	else if (ihandler.isKeyTapped(K_B))
	{
		g_pMixer->playAudio("sfx/click.wav", true);
		m_finished = true;
		m_pressedBack = true;
	}
	else if (ihandler.isKeyTapped(K_A))
	{
		g_pMixer->playAudio("sfx/click.wav", true);
		m_finished = true;
	}
	
	if (m_finished)
		m_onCompleteCallback(m_pressedBack);
}

void UIMoveSelect::render()
{
	UIComponent::render();
	g_pFont->renderString(m_moves[m_selectedMove]->getType(), MOVE_ACTIVE_TYPE_X * g_scale, MOVE_ACTIVE_TYPE_Y * g_scale);

	auto&& currPPToString = std::to_string(m_pps[m_selectedMove]);
	if (currPPToString.size() == 2)
		g_pFont->renderString(currPPToString, MOVE_ACTIVE_CURR_PP_X * g_scale - 8 * g_scale, MOVE_PP_Y * g_scale);
	else
		g_pFont->renderString(currPPToString, MOVE_ACTIVE_CURR_PP_X * g_scale, MOVE_PP_Y * g_scale);

	auto&& maxPPToString = std::to_string(m_moves[m_selectedMove]->getPP());
	if (maxPPToString.size() == 2)
		g_pFont->renderString(maxPPToString, MOVE_ACTIVE_MAX_PP_X * g_scale - 8 * g_scale, MOVE_PP_Y * g_scale);
	else
		g_pFont->renderString(maxPPToString, MOVE_ACTIVE_MAX_PP_X * g_scale, MOVE_PP_Y * g_scale);

	auto y = MOVE_FIRST_MOVE_Y * g_scale;
	for (auto i = 0; i < 4; ++i)
	{
		if (m_moves[i])
		{
			auto moveName = m_moves[i]->getMoveName();
			g_pFont->renderString(moveName, MOVE_FIRST_MOVE_X * g_scale, y);
			if (m_selectedMove == i) g_pFont->renderString("}", MOVE_SELECTION_ARROW_X * g_scale, y);
		}
		else
		{
			SDLRender(
				g_pRenderer, 
				m_noMoveTex->getTexture().get(),
				MOVE_FIRST_MOVE_X * g_scale, 
				y, 
				m_noMoveTex->getScaledWidth(),
				m_noMoveTex->getScaledHeight());
		}

		y += (Font::FONT_SIZE / 2) * g_scale;
	}
}

/* ===============================
   Internal Method Implementations
   =============================== */
static int8 getNumberOfValidMoves(const std::array<std::unique_ptr<Move>, 4>& moves)
{
	for (int i = 0; i < 4; ++i)
	{
		if (!moves[i])
			return i;
	}

	return 4;
}