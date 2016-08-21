/* ======================
   Date: 17/8/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once

#include "../mydef.h"

#include <string>
#include <vector>

class Move final
{
public:

	static bool isTypeSpecial(const std::string& typeName);
	static bool isMoveHighCrit(const std::string& moveName);

public:

	explicit Move(const std::string moveName);
	~Move();

	const std::string& getMoveName() const;
	const std::string& getType() const;
	const std::string& getEffect() const;

	const bool hasEffect() const;
	
	const int32 getPower() const;
	const int32 getAccuracy() const;
	const int32 getPP() const;

private:

	static const std::vector<std::string> s_specialTypes;
	static const std::vector<std::string> s_highCritMoves;

private:

	std::string m_name;
	std::string m_type;
	std::string m_effect;
	int32 m_power;
	int32 m_accuracy;
	int32 m_pp;
	
};
