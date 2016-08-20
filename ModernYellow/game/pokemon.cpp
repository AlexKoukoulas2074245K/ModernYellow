/* ======================
   Date: 28/7/2016
   Author: Alex Koukoulas
   ====================== */

#include "pokemon.h"
#include "move.h"
#include "pokemonstatutil.h"
#include "../jsonutil/json.h"
#include "../gameinfo.h"

#include <random>
#include <ctime>
extern pGameInfo_t g_pGameInfo;

/* ==============
   Public Methods
   ============== */
Pokemon::Pokemon(const std::string name, const int8 level):
    m_name(std::move(name)),
    m_level(level),
    m_status(STATUS_OK),    
    m_infoRoot(g_pGameInfo->getPokemonInfo(m_name))      
{    
    initIndividualValues();
    initEffortValues();
    initBaseStats();    
    initMiscInfo();
	initMoveSet();
    calculateCurrentStats();
}

Pokemon::~Pokemon() {}

void Pokemon::inflictDamage(const int32 damage) 
{
    m_currHp -= damage;

    if (m_currHp < 0)           
        m_currHp = 0;    
    if (m_currHp > m_currStats[S_HP]) 
        m_currHp = m_currStats[S_HP];    
}

const std::array<std::unique_ptr<Move>, 4>& Pokemon::getMoveSet() const
{
	return m_moveset;
}

const std::array<int8, 4>& Pokemon::getPPs() const
{
	return m_currPPs;
}

const std::string& Pokemon::getName() const
{
    return m_name;
}

const std::string& Pokemon::getID() const
{
    return m_id;
}

Pokemon::Status Pokemon::getStatus() const
{
    return m_status;
}

int8 Pokemon::getLevel() const
{
    return m_level;
}

int16 Pokemon::getCurrHp() const
{
    return m_currHp;
}

int16 Pokemon::getStat(const Stat& stat) const
{
    return m_currStats[stat];
}

bool Pokemon::hasFainted() const
{
    return m_currHp == 0;
}

/* ===============
   Private Methods
   =============== */
void Pokemon::initIndividualValues()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    m_individualValues[S_ATTACK]  = static_cast<uint8>(std::rand() % 16);
    m_individualValues[S_DEFENSE] = static_cast<uint8>(std::rand() % 16);
    m_individualValues[S_SPEED]   = static_cast<uint8>(std::rand() % 16);
    m_individualValues[S_SPECIAL] = static_cast<uint8>(std::rand() % 16);    
    m_individualValues[S_HP]      =  ((m_individualValues[S_ATTACK]  & 0x0001) << 3 |
                                      (m_individualValues[S_DEFENSE] & 0x0001) << 2 |
                                      (m_individualValues[S_SPEED]   & 0x0001) << 1 |
                                      (m_individualValues[S_SPECIAL] & 0x0001) << 0);
}

void Pokemon::initEffortValues()
{
    m_effortValues[S_HP]      = 0;
    m_effortValues[S_ATTACK]  = 0;
    m_effortValues[S_DEFENSE] = 0;
    m_effortValues[S_SPEED]   = 0;
    m_effortValues[S_SPECIAL] = 0;
}

void Pokemon::initBaseStats()
{
    m_baseStats[S_HP]      = m_infoRoot["hp"].asInt();
    m_baseStats[S_ATTACK]  = m_infoRoot["attack"].asInt();
    m_baseStats[S_DEFENSE] = m_infoRoot["defense"].asInt();
    m_baseStats[S_SPEED]   = m_infoRoot["speed"].asInt();
    m_baseStats[S_SPECIAL] = m_infoRoot["special"].asInt();
}

void Pokemon::initMiscInfo()
{    
    m_id = std::to_string(m_infoRoot["id"].asInt());

    // twice on purpose
    if (m_id.length() < 3) m_id = "0" + m_id;
    if (m_id.length() < 3) m_id = "0" + m_id;

    for (const auto& type : m_infoRoot["type"]) m_type.push_back(type["name"].asString());
    for (const auto& evol : m_infoRoot["evolution"])
    {
        m_evolution[0] = evol["name"].asString();
        m_evolution[1] = evol["method"].asString();
    }
    for (const auto& moveDesc : m_infoRoot["moveset"])
    {
        MoveLevelPair mlp = {};
        mlp.level    = moveDesc["level"].asInt();
        mlp.moveName = moveDesc["movename"].asString();
        m_learnset.push_back(std::move(mlp));
    }
    m_ovimageType = std::move(m_infoRoot["ovimagetype"].asString());
}

void Pokemon::initMoveSet()
{
	m_moveset = { nullptr, nullptr, nullptr, nullptr };
	auto index = 0;

	for (auto& moveLevelPair : m_learnset)
	{
		if (moveLevelPair.level <= m_level)
		{
			m_moveset[index] = std::make_unique<Move>(moveLevelPair.moveName);
			m_currPPs[index] = m_moveset[index]->getPP();
			index = (index + 1) % 4;
		}
	}
}

void Pokemon::calculateCurrentStats()
{
    m_currStats[S_HP] = stat_utils::calculateHPStat(m_level, m_baseStats[S_HP], m_individualValues[S_HP], m_effortValues[S_HP]);
    m_currStats[S_ATTACK] = stat_utils::calculateStatOtherThanHP(m_level, m_baseStats[S_ATTACK], m_individualValues[S_ATTACK], m_effortValues[S_ATTACK]);
    m_currStats[S_DEFENSE] = stat_utils::calculateStatOtherThanHP(m_level, m_baseStats[S_DEFENSE], m_individualValues[S_DEFENSE], m_effortValues[S_DEFENSE]);
    m_currStats[S_SPEED] = stat_utils::calculateStatOtherThanHP(m_level, m_baseStats[S_SPEED], m_individualValues[S_SPEED], m_effortValues[S_SPEED]);
    m_currStats[S_SPECIAL] = stat_utils::calculateStatOtherThanHP(m_level, m_baseStats[S_SPECIAL], m_individualValues[S_SPECIAL], m_effortValues[S_SPECIAL]);
    m_currHp = m_currStats[S_HP];
}