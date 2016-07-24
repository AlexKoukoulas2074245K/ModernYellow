/* ======================
   Date: 23/7/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once

#include "mydef.h"

class GameInfo
{
public:

    GameInfo();

    ~GameInfo();

    bool isReady() const;

private:

    bool readJsonData();

private:

    struct gameInfoImpl;
    std::unique_ptr<gameInfoImpl> m_impl;

};
