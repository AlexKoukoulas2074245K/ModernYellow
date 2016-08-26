#pragma once

#include "../../mydef.h"

#include <functional>
#include <utility>

class Move;
class ShakeGenerator final
{
public:
	ShakeGenerator(const Move&, const bool);

	const std::pair<int32, int32>& updateAndRetrieveOffset();

	bool isFinished() const;

private:

	enum ShakeType
	{
		SHAKE_DOWN,
		SHAKE_HOR_EXT,
		SHAKE_HOR_HEAVY,
		SHAKE_HOR_SHORT,
		SHAKE_HOR_LIGHT,
	};

private:

	ShakeType               m_shakeType;
	const Move&             m_usedMove;	
	bool                    m_isFinished;	
	bool                    m_directionIsRight;
	bool                    m_blockingStep;
	int32                   m_shakeStep;
	std::pair<int32, int32> m_offset;

};