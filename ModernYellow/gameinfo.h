/* ======================
   Date: 23/7/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once

#include "mydef.h"
#include <string>

namespace Json { class Value; }
class GameInfo
{
public:

    GameInfo();

    ~GameInfo();

    bool isReady() const;

    const Json::Value& getPokemonInfo(const std::string& name);
    
private:

    bool readJsonData();

private:

    struct gameInfoImpl;
    std::unique_ptr<gameInfoImpl> m_impl;

};
