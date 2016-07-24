/* ======================
   Date: 23/7/2016
   Author: Alex Koukoulas
   ====================== */

#include "gameinfo.h"
#include "jsonutil/json.h"

#include <fstream>

/* ===================
   Extern Declarations
   =================== */
extern std::string g_datPath;

/* ===================
   Struct Declarations
   =================== */
struct GameInfo::gameInfoImpl
{
    Json::Value m_root;    
    bool m_ready;
};

/* ==============
   Public Methods
   ============== */
GameInfo::GameInfo():
    m_impl(std::make_unique<GameInfo::gameInfoImpl>())
{
    m_impl->m_ready = readJsonData();    
}

GameInfo::~GameInfo(){}

bool GameInfo::isReady() const
{
    return m_impl->m_ready;
}

/* ===============
   Private Methods
   =============== */
bool GameInfo::readJsonData()
{
    Json::Reader jsonReader;
    std::ifstream jsonFile(g_datPath + "base_stats.json");

    return jsonReader.parse(jsonFile, m_impl->m_root);    
}