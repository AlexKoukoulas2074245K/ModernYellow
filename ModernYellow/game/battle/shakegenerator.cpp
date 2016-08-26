#include "shakegenerator.h"
#include "../move.h"

/* ==============
   Public Methods
   ============== */
ShakeGenerator::ShakeGenerator(const Move& moveUsed, const bool wasUsedByLocalPokemon)
	: m_usedMove(moveUsed)
	, m_isFinished(false)
	, m_offset({0, 0})
	, m_shakeStep(0)
	, m_directionIsRight(true)
	, m_blockingStep(true)
{
	if (wasUsedByLocalPokemon)
	{
		if (moveUsed.getPower() > 0 && moveUsed.hasEffect())
			m_shakeType = SHAKE_HOR_LIGHT;
		else if (moveUsed.getPower() <= 0)
			m_shakeType = SHAKE_HOR_SHORT;
	}
	else if (!wasUsedByLocalPokemon)
	{
		if (moveUsed.getPower() > 0 && !moveUsed.hasEffect())
			m_shakeType = SHAKE_DOWN;	
		else if (moveUsed.getPower() > 0 && moveUsed.hasEffect())
			m_shakeType = SHAKE_HOR_HEAVY;
		else
			m_shakeType = SHAKE_HOR_EXT;
	}
}

const std::pair<int32, int32>& ShakeGenerator::updateAndRetrieveOffset()
{
	switch (m_shakeType)	
	{
		case SHAKE_DOWN:
		{
			if (m_offset.second > 0)			
				--m_offset.second;			
			else
			{				
				if (++m_shakeStep == 5)				
					m_isFinished = true;					
				
				m_offset.second = 15 - (3 * m_shakeStep);
			}
		} break;

		case SHAKE_HOR_EXT:
		{
			if (m_directionIsRight)
			{
				if (!m_blockingStep)
					++m_offset.first;
				
				m_blockingStep = !m_blockingStep;
				
				if (m_offset.first >= 6)
				{
					m_directionIsRight = false;					
					++m_shakeStep;
				}
			}
			else
			{
				if (!m_blockingStep)
					--m_offset.first;

				m_blockingStep = !m_blockingStep;

				if (m_offset.first <= 0)
				{
					m_directionIsRight = true;					
					if (++m_shakeStep == 4)
						m_isFinished = true;
				}
			}

		} break;

		case SHAKE_HOR_HEAVY:
		{
			if (m_offset.first > 0)			
				--m_offset.first;			
			else
			{				
				if (++m_shakeStep == 5)
					m_isFinished = true;
				m_offset.first = 15 - (3 * m_shakeStep);
			}

		} break;

		case SHAKE_HOR_SHORT:
		{			
			if (--m_offset.first <= 0)
			{
				if (m_shakeStep == 0 || m_shakeStep == 1)
				{
					m_offset.first = 6;					
					++m_shakeStep;
				}
				else if (m_shakeStep == 2)			
					m_isFinished = true;				
			}
		} break;

		case SHAKE_HOR_LIGHT:
		{			
			if (--m_offset.first <= 0)
			{
				if (m_shakeStep == 0 || m_shakeStep == 1)
				{
					m_offset.first = 3;
					++m_shakeStep;
				}
				else if (m_shakeStep == 2)
					m_isFinished = true;
			}
		} break;		
	}

	return m_offset;
}

bool ShakeGenerator::isFinished() const
{
	return m_isFinished;
}
