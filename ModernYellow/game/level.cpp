/* ======================
   Date: 15/6/2016
   Author: Alex Koukoulas
   ====================== */


#include "level.h"
#include "tile.h"
#include "npc.h"
#include "overworldobject.h"
#include "../resources/sresmanager.h"
#include "../resources/dataresource.h"
#include "../resources/textureresource.h"
#include "../strutils.h"

#include <SDL_timer.h>
#include <SDL_log.h>
#include <SDL_render.h>
#include <SDL_assert.h>
#include <set>

//define LEVEL_DEBUG_OCC
#if (defined (_DEBUG) || defined (DEBUG)) && defined (LEVEL_DEBUG_OCC)
#include <SDL_gfxPrimitives.h>
#endif


#define CORRUPTED_LEVEL() {SDL_FORCE_DISPLAY_ERROR("Corrupted level file: " + m_name); return false;}

extern string g_datPath;
extern string g_texPath;
extern uint32 g_scale;
extern uint32 g_tileSize;
extern pRenderer_t g_pRenderer;

/* ==============
   Public Methods
   ============== */
Level::Level(
    const string& levelName,    
    const std::shared_ptr<TextureResource>& pAtlas):

    m_name(levelName),
    m_pOverworldAtlas(pAtlas),
    m_ready(false),    
    m_warpLevel(0),
    m_warpLevelDelay(LEVEL_WARP_LEVEL_DELAY),
    m_rows(0),
    m_cols(0),
    m_xOffset(0),
    m_yOffset(0)   
{    
    if (!loadLevelTex()) return;
    if (!readLevelData()) return;            
    if (!readLevelObjects()) return;
    if (!readLevelWarps()) return;

    // Calculate level area
    m_levelArea.x = 0;
    m_levelArea.y = 0;
    m_levelArea.w = m_pLevelTex->getSurface().get()->w * g_scale;
    m_levelArea.h = m_pLevelTex->getSurface().get()->h * g_scale;
    m_ready = true;
}

Level::~Level() = default;

void Level::update()
{
    if (m_warpLevel > 0 && --m_warpLevelDelay <= 0)
    {
        m_warpLevelDelay = LEVEL_WARP_LEVEL_DELAY;
        m_warpLevel++;
       
        auto normXOffset = -m_xOffset / (int32)g_scale;
        auto normYOffset = -m_yOffset / (int32)g_scale;

        SDL_Rect darkenArea = {};        
        auto stdWidth = int32(GAME_COLS * DEFAULT_TILE_SIZE);
        auto stdHeight = int32(GAME_ROWS * DEFAULT_TILE_SIZE);

        darkenArea.x = normXOffset < 0 || normXOffset + stdWidth > m_pLevelTex->getSurface()->w ? 0 : normXOffset;
        darkenArea.y = normYOffset < 0 || normYOffset + stdHeight> m_pLevelTex->getSurface()->h ? 0 : normYOffset;
        darkenArea.w = normXOffset < 0 || normXOffset + stdWidth > m_pLevelTex->getSurface()->w ? m_pLevelTex->getSurface()->w : stdWidth;
        darkenArea.h = normYOffset < 0 || normYOffset + stdHeight> m_pLevelTex->getSurface()->h ? m_pLevelTex->getSurface()->h :  stdHeight;
               
#//if !defined(DEBUG) && !defined(_DEBUG)
        m_pLevelTex->darken(darkenArea);        
//#endif

        for (const auto& npc: m_npcs)
        {
            npc->darken();
        }

        for (const auto& owo: m_owobjects)
        {
            owo->darken();
        }

        for (const auto& seaTile : m_seaTiles)
        {
            seaTile->darken();
        }

        for (const auto& flowerTile : m_flowerTiles)
        {
            flowerTile->darken();
        }

        if (m_warpLevel == LEVEL_WARP_LEVEL_MAX)
        {
            if (!loadLevelTex()) return;
            if (!readLevelData()) return;
            if (!readLevelObjects()) return;
            if (!readLevelWarps()) return;
            if (!loadNPCData()) return;

            // Calculate level area
            m_levelArea.x = 0;
            m_levelArea.y = 0;
            m_levelArea.w = m_pLevelTex->getSurface().get()->w * g_scale;
            m_levelArea.h = m_pLevelTex->getSurface().get()->h * g_scale;
            m_ready = true;

        }
    }

    for (const auto& npc : m_npcs)
    {
        npc->update();
    }    

    for (const auto& seaTile: m_seaTiles)
    {
        seaTile->update();
    }

    for (const auto& flowerTile: m_flowerTiles)
    {
        flowerTile->update();
    }

    for (const auto& owobject: m_owobjects)
    {
        owobject->update();
    }
}

void Level::render()
{
    // First layer is the sea tiles beneath everything else
    for (const auto& seaTile : m_seaTiles)
    {
        seaTile->render(m_xOffset, m_yOffset);         
    }

    // Then the level texture is rendered on top
    SDL_RenderCopy(g_pRenderer.get(), m_pLevelTex->getTexture().get(), nullptr, &m_levelArea);
     
#if (defined (_DEBUG) || defined (DEBUG)) && defined (LEVEL_DEBUG_OCC)
     for (size_t y = 0; y < m_rows; ++y)
     {
         for (size_t x = 0; x < m_cols; ++x)
         {
             if (!m_tilemap[y][x]->isWalkable() && m_tilemap[y][x]->getTileType() != Tile::TT_SOLID)
             {                 
                 auto worldX = m_tilemap[y][x]->getX();
                 auto worldY = m_tilemap[y][x]->getY();
                 filledCircleColor(
                     g_pRenderer.get(),
                     (worldX + g_tileSize/2 + m_xOffset), 
                     (worldY + g_tileSize/2 + m_yOffset), 
                     g_tileSize/2,
                     envcolors::EC_BLACK);
             }
         }
     }       
#endif
    
    // then the flower tiles
    for (const auto& flowerTile: m_flowerTiles)
    {         
        flowerTile->render(m_xOffset, m_yOffset);         
    }
     
    // then all the overworld objects
    for (const auto& owobject: m_owobjects)
    {
        owobject->render();
    }

    // then all the npcs
    for (const auto& npc : m_npcs)
    {
        npc->render();
    }
}

void Level::renderEncOccTiles()
{
    for (const auto& encTile : m_encounterTiles)
    {
        if (encTile->isOccupied())
        {
            encTile->render(m_xOffset, m_yOffset);
        }
    }
}

void Level::startWarpTo(std::shared_ptr<Warp> destination)
{
    setFrozenNpcs(true);
    m_warpLevel = 1;
    m_name = destination->location;            
}

bool Level::isInnerDoor(std::shared_ptr<Tile> tile) const
{
    auto pLeftTile  = getTileLeftOf(tile);
    auto pRightTile = getTileRightOf(tile);

    if (tile->getWarp() && pLeftTile && pLeftTile->getWarp()) return true;
    if (tile->getWarp() && pRightTile && pRightTile->getWarp()) return true;

    return false;
}

bool Level::isReady() const
{
    return m_ready;
}

int32 Level::getWarpLevel() const
{
    return m_warpLevel;
}

bool Level::loadNPCData()
{
    m_npcs.clear();

    // Load the npc data resource
    auto pNPCResource = castResToData(resmanager.loadResource("npcs.dat", RT_DATA));

    if (!pNPCResource) return false;

    // Get Line by Line content
    auto pContent = pNPCResource->getContent();

    for (size_t lineIndex = 0; lineIndex < pContent.size(); lineIndex += 2)
    {
        const auto& line = pContent[lineIndex];
        const auto& dialogue = pContent[lineIndex + 1];

        if (string_utils::startsWith(line, m_name))
        {
            auto lineComps   = string_utils::split(line, ' ');
            auto vecPosComps = string_utils::split(lineComps[1], ',');
            auto texUVComps  = string_utils::split(lineComps[2], ',');
            auto movingNpc   = (lineComps[3] == "true") ? true : false;
            auto dir         = (Direction)(std::atoi(lineComps[4].c_str()));
            auto isTrainer   = (lineComps[5] == "true") ? true : false;
           
            m_npcs.push_back(std::make_shared<Npc>(
                std::atoi(texUVComps[0].c_str()) * DEFAULT_TILE_SIZE,
                std::atoi(texUVComps[1].c_str()) * DEFAULT_TILE_SIZE,
                getTileRC(std::atoi(vecPosComps[0].c_str()), std::atoi(vecPosComps[1].c_str())),
                dir,
                shared_from_this(),
                m_pOverworldAtlas,
                movingNpc,                
                isTrainer,
                dialogue));
        }
    }    

    return true;
}

std::shared_ptr<Npc> Level::getNpcAt(const std::shared_ptr<Tile>& tile) const
{
    return getNpcAtRC(tile->getCol(), tile->getRow());
}

std::shared_ptr<Npc> Level::getNpcAtRC(const uint32 col, const uint32 row) const
{
    for (auto npc: m_npcs)
    {
        if (npc->getCurrTile()->getCol() == col &&
            npc->getCurrTile()->getRow() == row)
        {
            return npc;
        }
    }

    return nullptr;
}

std::shared_ptr<OWObject> Level::getOWObjectAt(const std::shared_ptr<Tile>& tile) const
{
    return getOWObjectAtRC(tile->getCol(), tile->getRow());
}

std::shared_ptr<OWObject> Level::getOWObjectAtRC(const uint32 col, const uint32 row) const
{
    for (auto owobject: m_owobjects)
    {
        if (owobject->getCurrTile()->getCol() == col &&
            owobject->getCurrTile()->getRow() == row)
        {
            return owobject;
        }
    }

    return nullptr;
}

std::shared_ptr<Tile> Level::getTileXY(
    const int32 worldX,
    const int32 worldY) const
{    
    if (
        worldX < 0 && 
        worldX >= m_levelArea.w &&
        worldY < 0 && 
        worldY >= m_levelArea.h) return nullptr;

    return m_tilemap[worldY / g_tileSize][worldX / g_tileSize];
}

std::shared_ptr<Tile> Level::getTileRC(
    const uint32 col,
    const uint32 row) const
{
    if(
        col < 0 &&
        col >= m_cols &&
        row < 0 &&
        row >= m_rows) return nullptr;

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
    
    for (const auto& npc : m_npcs)
    {
        npc->setOffset(xOffset, yOffset);
    }

    for (const auto& owobject: m_owobjects)
    {
        owobject->setOffset(xOffset, yOffset);
    }
}

void Level::setFrozenNpcs(const bool frozen)
{
    for (auto npc: m_npcs)
    {
        npc->setFrozen(frozen);
    }
}

void Level::resetWarping()
{
    m_warpLevel = 0;
}

/* ===============
   Private Methods
   =============== */
bool Level::loadLevelTex()
{
    m_pLevelTex = castResToTex(resmanager.loadResource("levels/" + m_name + ".png", RT_TEXTURE));
    return m_pLevelTex ? true : false;
}

bool Level::readLevelData()
{   
    // Load the dataresource 
    auto pLevelResource = castResToData(resmanager.loadResource("levels/" + m_name + ".pkm", RT_DATA));

    m_tilemap.clear();

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

        // Add animated tiles to respective vectors
        auto pTile = std::make_shared<Tile>(colIndex, rowIndex, tileType);
        m_tilemap[rowIndex][colIndex] = pTile;
        
        if (tileType == Tile::TT_SEA)
        {
            m_seaTiles.push_back(pTile);
        }
        else if (
            tileType == Tile::TT_FLOWER_1 || 
            tileType == Tile::TT_FLOWER_2)
        {
            m_flowerTiles.push_back(pTile);
        }
        else if(tileType == Tile::TT_ENCOUNTER)
        {
            m_encounterTiles.push_back(pTile);
        }

        colIndex++;

        if (colIndex >= m_cols)
        {
            colIndex = 0;
            rowIndex++;
            m_tilemap.push_back(std::vector<std::shared_ptr<Tile>>(m_cols));
        }
    }

    if (m_name[0] == 'i') return true;

    // Add the extra needed sea tiles to create the illusion of
    // the moving current in the sea tiles
    std::set<std::shared_ptr<Tile>> perimetricalSeaTiles;

    for (const auto& aniTile : m_seaTiles)
    {
        if (aniTile->getTileType() == Tile::TT_SEA)
        {
            // Get Horizontal and Vertical Tiles
            auto pTileLeftOf = getTileLeftOf(aniTile);
            auto pTileRightOf = getTileRightOf(aniTile);
            auto pTileAbove = getTileAbove(aniTile);
            auto pTileBelow = getTileBelow(aniTile);

            // Get Diagonal Tiles
            auto pTileUL = getTileRC(aniTile->getCol() - 1, aniTile->getRow() - 1);
            auto pTileUR = getTileRC(aniTile->getCol() + 1, aniTile->getRow() - 1);
            auto pTileDL = getTileRC(aniTile->getCol() - 1, aniTile->getRow() + 1);
            auto pTileDR = getTileRC(aniTile->getCol() + 1, aniTile->getRow() + 1);

            // Test horizontal and vertical tiles
            if (pTileLeftOf && pTileLeftOf->getTileType() != Tile::TT_SEA) perimetricalSeaTiles.insert(pTileLeftOf);
            if (pTileRightOf && pTileRightOf->getTileType() != Tile::TT_SEA) perimetricalSeaTiles.insert(pTileRightOf);
            if (pTileAbove && pTileAbove->getTileType() != Tile::TT_SEA) perimetricalSeaTiles.insert(pTileAbove);
            if (pTileBelow && pTileBelow->getTileType() != Tile::TT_SEA) perimetricalSeaTiles.insert(pTileBelow);
            
            // Test Diagonal Tiles
            if (pTileUL && pTileUL->getTileType() != Tile::TT_SEA) perimetricalSeaTiles.insert(pTileUL);
            if (pTileUR && pTileUR->getTileType() != Tile::TT_SEA) perimetricalSeaTiles.insert(pTileUR);
            if (pTileDL && pTileDL->getTileType() != Tile::TT_SEA) perimetricalSeaTiles.insert(pTileDL);
            if (pTileDR && pTileDR->getTileType() != Tile::TT_SEA) perimetricalSeaTiles.insert(pTileDR);
        }
    }

    for (auto pTile : perimetricalSeaTiles)
    {
        pTile->addSeaTile();
        m_seaTiles.push_back(pTile);
    }

    return true;
}

bool Level::readLevelObjects()
{
    // Load the objects data resource
    auto pOWOResource = castResToData(resmanager.loadResource("objects.dat", RT_DATA));
    
    if (!pOWOResource) return false;

    m_owobjects.clear();

    // Get Line by Line content
    auto content = pOWOResource->getContent();

    for (size_t lineIndex = 0; lineIndex < content.size(); lineIndex += 2)
    {
        const auto& line = content[lineIndex];
        const auto& dialogue = content[lineIndex + 1];

        if (string_utils::startsWith(line, m_name))
        {
            auto lineComps   = string_utils::split(line, ' ');
            auto vecPosComps = string_utils::split(lineComps[1], ',');
            auto texUVComps  = string_utils::split(lineComps[2], ',');
            auto obtainable  = (lineComps[3] == "true") ? true : false;
            auto solid       = (lineComps[4] == "true") ? true : false;
            auto cuttable    = (lineComps[5] == "true") ? true : false;
           
            m_owobjects.push_back(std::make_shared<OWObject>(
                std::atoi(texUVComps[0].c_str()) * DEFAULT_TILE_SIZE,
                std::atoi(texUVComps[1].c_str()) * DEFAULT_TILE_SIZE,
                m_pOverworldAtlas,
                getTileRC(std::atoi(vecPosComps[0].c_str()), std::atoi(vecPosComps[1].c_str())),                
                dialogue,
                obtainable,
                solid,
                cuttable));
        }
    }    

    return true;
}

bool Level::readLevelWarps()
{
    auto warpResource = castResToData(resmanager.loadResource("warps.dat", RT_DATA));

    auto content = warpResource->getContent();

    for (const auto& line: content)
    {
        auto vecComps    = string_utils::split(line, '>');
        auto vecLhsComps = string_utils::split(vecComps[0], ' ');
        auto vecRhsComps = string_utils::split(vecComps[1], ' ');
        
        auto lhsLocation = vecLhsComps[0];
        auto rhsLocation = vecRhsComps[0];
            
        if (lhsLocation != m_name && rhsLocation != m_name) continue;

        auto lhsCoords = string_utils::split(vecLhsComps[1], ',');
        auto rhsCoords = string_utils::split(vecRhsComps[1], ',');

        auto lhsLocCol = std::atoi(lhsCoords[0].c_str());
        auto lhsLocRow = std::atoi(lhsCoords[1].c_str());

        auto rhsLocCol = std::atoi(rhsCoords[0].c_str());
        auto rhsLocRow = std::atoi(rhsCoords[1].c_str());

        auto lhsWarp      = std::make_shared<Warp>();
        lhsWarp->location = lhsLocation;
        lhsWarp->col      = lhsLocCol;
        lhsWarp->row      = lhsLocRow;

        auto rhsWarp      = std::make_shared<Warp>();
        rhsWarp->location = rhsLocation;
        rhsWarp->col      = rhsLocCol;
        rhsWarp->row      = rhsLocRow;

        // Attach a warp (to the target location) to the respective
        // tile found
        if (lhsLocation == m_name)
        {            
            if (lhsLocation[0] == 'i' && rhsLocation[0] == 'o')                 
            {
                rhsWarp->forcedDir = DIR_DOWN;
            }
            else if (lhsLocation[0] == 'o' && rhsLocation[0] == 'i') 
            {
                rhsWarp->forcedDir = DIR_UP;
            }
            else 
            {
                rhsWarp->forcedDir = -1;
            }

            getTileRC(lhsWarp->col, lhsWarp->row)->addWarp(rhsWarp);
        }
        else
        {
            if (rhsLocation[0] == 'i' && lhsLocation[0] == 'o')
            {
                lhsWarp->forcedDir = DIR_DOWN;
            }
            else if (rhsLocation[0] == 'o' && lhsLocation[0] == 'i')
            {
                lhsWarp->forcedDir = DIR_UP;
            }                            
            else 
            {
                lhsWarp->forcedDir = -1;
            }

            getTileRC(rhsWarp->col ,rhsWarp->row)->addWarp(lhsWarp);
        }
    }

    return true;
}
