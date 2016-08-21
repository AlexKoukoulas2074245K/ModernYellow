/* ======================
   Date: 17/8/2016
   Author: Alex Koukoulas
   ====================== */

#include "move.h"
#include "../gameinfo.h"
#include "../jsonutil/json.h"

#include <utility>
#include <algorithm>

extern pGameInfo_t g_pGameInfo;

/* =============
   Class Methods
   ============= */
bool Move::isTypeSpecial(const std::string& typeName)
{
	return std::find(s_specialTypes.begin(), s_specialTypes.end(), typeName) != s_specialTypes.end();
}

bool Move::isMoveHighCrit(const std::string& moveName)
{
	return std::find(s_highCritMoves.begin(), s_highCritMoves.end(), moveName) != s_highCritMoves.end();
}

/* =====================
   Private Class Objects
   ===================== */
const std::vector<std::string> Move::s_specialTypes = { "WATER", "GRASS", "FIRE", "ICE", "ELECTRIC", "PSYCHIC", "DRAGON" };
const std::vector<std::string> Move::s_highCritMoves = { "CRABHAMMER", "KARATE_CHOP", "RAZOR_LEAF", "RAZOR_WIND", "SLASH", "SKY_ATTACK" };

/* ==============
   Public Methods
   ============== */
Move::Move(const std::string moveName)
	: m_name(std::move(moveName))
{
	auto& moveRoot = g_pGameInfo->getMoveInfo(m_name);
	m_accuracy = moveRoot["accuracy"].asInt();
	m_power = moveRoot["power"].asInt();
	m_pp = moveRoot["pp"].asInt();
	m_type = moveRoot["type"].asString();
	
	if (moveRoot.isMember("effect"))
		m_effect = moveRoot["effect"].asString();
}

Move::~Move() {}

const std::string& Move::getMoveName() const
{
	return m_name;
}

const std::string& Move::getType() const
{
	return m_type;
}

const std::string& Move::getEffect() const
{
	return m_effect;
}

const bool Move::hasEffect() const
{
	return m_effect.size() != 0;
}

const int32 Move::getPower() const
{
	return m_power;
}

const int32 Move::getAccuracy() const
{
	return m_accuracy;
}

const int32 Move::getPP() const
{
	return m_pp;
}

