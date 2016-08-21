/* ======================
   Date: 28/7/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once
#include "../mydef.h"
#include <string>
#include <vector>
#include <array>

struct MoveLevelPair
{    
    int8 level;
    std::string moveName;
};

namespace Json { class Value; }
class Move;
class Pokemon
{
public:

    enum Stat
    {
        S_HP = 0,
        S_ATTACK = 1,
        S_DEFENSE = 2,
        S_SPEED = 3,
        S_SPECIAL = 4
    };

    enum Status
    {
        STATUS_OK,
        STATUS_POISONED,
        STATUS_BURNED,
        STATUS_FROZEN,
        STATUS_PARALYZED
    };

    Pokemon(const std::string name, const int8 level);

	~Pokemon();

    void inflictDamage(const int32 damage);

	const std::array<std::unique_ptr<Move>, 4>& getMoveSet() const;
	
	const std::array<int8, 4>& getPPs() const;

    const std::string& getName() const;

    const std::string& getID() const;

	const std::vector<std::string>& getType() const;

    Status getStatus() const;

    int8 getLevel() const;

    int16 getCurrHp() const;

    int16 getStat(const Stat& stat) const;

	int16 getBaseStat(const Stat& stat) const;

    bool hasFainted() const;    

private:

    void initIndividualValues();
    void initEffortValues();
    void initBaseStats();
    void initMiscInfo();
	void initMoveSet();

    void calculateCurrentStats();

private:

    std::string                          m_name;
    std::string                          m_id;
    int8                                 m_level;
    std::string                          m_ovimageType;
    std::vector<std::string>             m_type;
    std::array<std::string, 2>           m_evolution;
    std::vector<MoveLevelPair>           m_learnset;
    std::array<uint8, 5>                 m_individualValues;
    std::array<uint16, 5>                m_effortValues;
    std::array<int16, 5>                 m_baseStats;    
    std::array<int16, 5>                 m_currStats;
	std::array<std::unique_ptr<Move>, 4> m_moveset;
	std::array<int8, 4>                  m_currPPs;
    int16                                m_currHp;
    Status                               m_status;

    const Json::Value& m_infoRoot;
};