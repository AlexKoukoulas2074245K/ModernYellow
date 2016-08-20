/* ======================
   Date: 14/8/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once

#include "uicomponent.h"

#include <functional>
#include <array>

class TextureResource;
class Move;
class UIMoveSelect: public UIComponent
{
public:
	UIMoveSelect(
		const std::array<std::unique_ptr<Move>, 4>&,
		const std::array<int8, 4>&,
		const std::function<void(const bool)>,
		int8&);
	
	void update() override;

	void render() override;

private:

	std::shared_ptr<TextureResource>            m_noMoveTex;
	const std::array<std::unique_ptr<Move>, 4>& m_moves;
	const std::array<int8, 4>&                  m_pps;
	int8&                                       m_selectedMove;
	bool                                        m_pressedBack;
	const std::function<void(const bool)>       m_onCompleteCallback;
	const int8                                  m_numValidMoves;
};
