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
    Json::Value m_pkmnRoot;    
    Json::Value m_itemRoot;
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

const Json::Value& GameInfo::getPokemonInfo(const std::string& name)
{
    return m_impl->m_pkmnRoot[name];
}

/* ===============
   Private Methods
   =============== */
bool GameInfo::readJsonData()
{
    Json::Reader jsonPkmnReader;
    std::ifstream jsonPkmnFile(g_datPath + "base_stats.json");

    Json::Reader jsonItemReader;
    std::ifstream jsonItemFile(g_datPath + "items.json");

    return jsonPkmnReader.parse(jsonPkmnFile, m_impl->m_pkmnRoot) &&
           jsonItemReader.parse(jsonItemFile, m_impl->m_itemRoot);    
}