/* ======================
   Date: 15/6/2016
   Author: Alex Koukoulas
   ====================== */


#include "level.h"
#include "tile.h"
#include "sprite.h"
#include "../resources/sresmanager.h"
#include "../resources/dataresource.h"
#include "../resources/textureresource.h"
#include "../strutils.h"
#include <SDL_log.h>
#include <SDL_render.h>
#include <SDL_assert.h>

#define CORRUPTED_LEVEL() {SDL_FORCE_DISPLAY_ERROR("Corrupted level file: " + m_name); return false;}

extern string g_datPath;
extern string g_texPath;
extern uint32 g_scale;
extern uint32 g_tileSize;
extern SDL_Renderer* g_renderer;

/* ==============
   Public Methods
   ============== */
Level::Level(
    const string& levelName,    
    const std::shared_ptr<TextureResource>& pAtlas):

    m_name(levelName),
    m_ready(false),
    m_rows(0),
    m_cols(0),
    m_xOffset(0),
    m_yOffset(0)
{    
    if (!loadLevelTex()) return;
    if (!readLevelData()) return;            

    // Calculate level area
    m_levelArea.x = 0;
    m_levelArea.y = 0;
    m_levelArea.w = m_levelTex->getSurface().get()->w * g_scale;
    m_levelArea.h = m_levelTex->getSurface().get()->h * g_scale;
    m_ready = true;
}

Level::~Level() = default;

void Level::render() const
{
     SDL_RenderCopy(g_renderer, m_levelTex->getTexture().get(), nullptr, &m_levelArea);
}

bool Level::isReady() const
{
    return m_ready;
}

void Level::getNPCData(
    const std::shared_ptr<TextureResource>& pAtlas,
    std::vector<std::unique_ptr<Sprite>>& outNpcs) const
{
    // Load the dataresource 
    auto pNPCResource = castResToData(resmanager.loadResource("npcs.dat", RT_DATA));

    // Get Line by Line content
    auto pContent = pNPCResource->getContent();

    for (size_t lineIndex = 0; lineIndex < pContent.size(); lineIndex += 2)
    {
        const auto& line = pContent[lineIndex];

        if (string_utils::startsWith(line, m_name))
        {
            auto lineComps   = string_utils::split(line, ' ');
            auto vecPosComps = string_utils::split(lineComps[1], ',');
            auto texUVComps  = string_utils::split(lineComps[2], ',');
            auto strMoving   = lineComps[3];
            auto strDir      = lineComps[4];
            auto strTrainer  = lineComps[5];
           
            outNpcs.push_back(std::make_unique<Sprite>(
                std::atoi(texUVComps[0].c_str()) * DEFAULT_TILE_SIZE,
                std::atoi(texUVComps[1].c_str()) * DEFAULT_TILE_SIZE,
                getTileRC(std::atoi(vecPosComps[0].c_str()), std::atoi(vecPosComps[1].c_str())),
                shared_from_this(),
                pAtlas));
        }
    }    
}

std::shared_ptr<Tile> Level::getTileXY(
    const int32 worldX,
    const int32 worldY) const
{    
    SDL_assert(
        worldX >= 0 && 
        worldX < m_levelArea.w &&
        worldY >= 0 && 
        worldY < m_levelArea.h);

    return m_tilemap[worldY / g_tileSize][worldX / g_tileSize];
}

std::shared_ptr<Tile> Level::getTileRC(
    const uint32 col,
    const uint32 row) const
{
    SDL_assert(
        col >= 0 &&
        col < m_cols &&
        row >= 0 &&
        row < m_rows);

    return m_tilemap[row][col];
}

std::shared_ptr<Tile> Level::getTileLeftOf(const std::shared_ptr<Tile> tile) const
{
    return getTileRC(tile->getCol() - 1, tile->getRow());
}

std::shared_ptr<Tile> Level::getTileRightOf(const std::shared_ptr<Tile> tile) const
{
    return getTileRC(tile->getCol() + 1, tile->getRow());
}

std::shared_ptr<Tile> Level::getTileAbove(const std::shared_ptr<Tile> tile) const
{
    return getTileRC(tile->getCol(), tile->getRow() - 1);
}

std::shared_ptr<Tile> Level::getTileBelow(const std::shared_ptr<Tile> tile) const
{
    return getTileRC(tile->getCol(), tile->getRow() + 1);
}

void Level::setOffset(
    const int32 xOffset /* 0 */, 
    const int32 yOffset /* 0 */)
{
    m_xOffset = xOffset;
    m_yOffset = yOffset;

    m_levelArea.x = m_xOffset;
    m_levelArea.y = m_yOffset;
}

/* ===============
   Private Methods
   =============== */
bool Level::loadLevelTex()
{
    m_levelTex = castResToTex(resmanager.loadResource("levels/" + m_name + ".png", RT_TEXTURE));
    return m_levelTex ? true : false;
}

bool Level::readLevelData()
{   
    // Load the dataresource 
    auto pLevelResource = castResToData(resmanager.loadResource("levels/" + m_name + ".pkm", RT_DATA));

    // Get vector of strings
    auto levelData = pLevelResource->getContent();

    if (levelData.size() < 3) CORRUPTED_LEVEL();   

    // Skip headers
    uint32 dataIndex = 2;
    
    // Get rows and cols
    auto comps = string_utils::split(levelData[dataIndex++], ',');
    if (comps.size() < 2) CORRUPTED_LEVEL();
    m_cols = std::atoi(comps[0].c_str());
    m_rows = std::atoi(comps[1].c_str());

    m_tilemap.push_back(std::vector<std::shared_ptr<Tile>>(m_cols));
    uint32 rowIndex = 0;
    uint32 colIndex = 0;

    // Gather the rest of the level data
    for (; dataIndex < levelData.size(); ++dataIndex)
    {
        // The line format is <texu, texv> <tiletype>.       
        auto lineComps = string_utils::split(levelData[dataIndex], ' ');

        // Grab the tex coords (first entry in line) and split on comma
        auto strTexCoords = string_utils::split(lineComps[0], ',');
        auto texCol = std::atoi(strTexCoords[0].c_str());
        auto texRow = std::atoi(strTexCoords[1].c_str());

        // Grab the tile type
        uint32 tileType = std::atoi(lineComps[1].c_str());

        m_tilemap[rowIndex][colIndex] = std::make_unique<Tile>(colIndex, rowIndex, tileType);
        colIndex++;

        if (colIndex >= m_cols)
        {
            colIndex = 0;
            rowIndex++;
            m_tilemap.push_back(std::vector<std::shared_ptr<Tile>>(m_cols));
        }
    }

    return true;
}
