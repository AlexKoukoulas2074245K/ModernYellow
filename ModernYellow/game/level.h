/* ======================
   Date: 15/6/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once
#include "../portcommon.h"
#include "../mydef.h"

#include <unordered_map>
#include <vector>

using std::string;

struct Warp
{
    int32 forcedDir;
    string location;
    bool routeConnection;
    uint32 col, row;
};

class Tile;
class TextureResource;
class Npc;
class OWObject;
class Level: public std::enable_shared_from_this<Level>
{    
public:

    using tilemap_t = std::vector<std::vector<std::shared_ptr<Tile>>>;
    using anitilemap_t = std::vector<std::shared_ptr<Tile>>;
    using npcs_t = std::vector<std::shared_ptr<Npc>>;
    using owobjects_t = std::vector<std::shared_ptr<OWObject>>;    

public:

    static std::unordered_map<string, uint32> s_locationColors;

public:

    static const uint32 LEVEL_WARP_LEVEL_DELAY = 8;
    static const uint32 LEVEL_WARP_LEVEL_MAX = 5;

public:
    explicit Level(
        const string& levelName, 
        const std::shared_ptr<TextureResource>& pAtlas);
    
    ~Level();

    void update();

    void render();

    void renderEncOccTiles();    

    void startWarpTo(std::shared_ptr<Warp> destination);
    
    bool isInnerDoor(std::shared_ptr<Tile> tile) const;

    uint32 getCurrColor() const;

    bool isReady() const;

    int getWarpLevel() const;

    // Needs to be called after Level construction, as there is use
    // of shared_from_this and during construction no shared_ptr is 
    // pointing to this instance
    bool loadNPCData();    
    
    std::shared_ptr<Npc> getNpcAt(const std::shared_ptr<Tile>& tile) const;

    std::shared_ptr<Npc> getNpcAtRC(const uint32 col, const uint32 row) const;

    std::shared_ptr<OWObject> getOWObjectAt(const std::shared_ptr<Tile>& tile) const;

    std::shared_ptr<OWObject> getOWObjectAtRC(const uint32 col, const uint32 row) const;

    std::shared_ptr<Tile> getTileXY(
        const int32 worldX,
        const int32 worldY) const;
    
    std::shared_ptr<Tile> getTileRC(
        const uint32 col,
        const uint32 row) const;

    std::shared_ptr<Tile> getTileLeftOf(const std::shared_ptr<Tile> tile)  const;
    std::shared_ptr<Tile> getTileRightOf(const std::shared_ptr<Tile> tile) const;
    std::shared_ptr<Tile> getTileAbove(const std::shared_ptr<Tile> tile)   const;
    std::shared_ptr<Tile> getTileBelow(const std::shared_ptr<Tile> tile)   const;

    void setOffset( 
        const int32 xOffset = 0, 
        const int32 yOffset = 0);

    void setFrozenNpcs(const bool frozen);

    void resetWarping();

private:
    
    void switchPaletteTo(const uint32 color);

    bool loadLevelTex();

    bool readLevelData();    

    bool readLevelObjects();

    bool readLevelWarps();

private:

    bool m_ready;    
    bool m_outdoors;
    string m_name;
    
    tilemap_t m_tilemap;    
    anitilemap_t m_seaTiles;
    anitilemap_t m_flowerTiles;
    anitilemap_t m_encounterTiles;

    npcs_t m_npcs;
    owobjects_t m_owobjects;

    std::shared_ptr<TextureResource> m_pLevelTex;      
    std::shared_ptr<TextureResource> m_pOverworldAtlas;

    std::shared_ptr<Warp> m_pCurrDestination;

    SDL_Rect m_levelArea;
    uint32 m_rows, m_cols;
    uint32 m_currColor;
    int32 m_xOffset, m_yOffset;

    int32 m_warpLevelDelay;
    int m_warpLevel;
};