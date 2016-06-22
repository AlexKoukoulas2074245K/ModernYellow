/* ======================
   Date: 15/6/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once
#include "../portcommon.h"
#include "../mydef.h"

#include <vector>

using std::string;
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

    explicit Level(
        const string& levelName, 
        const std::shared_ptr<TextureResource>& pAtlas);
    
    ~Level();

    void update();

    void render();

    void renderEncOccTiles();    

    bool isReady() const;

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

private:

    bool loadLevelTex();

    bool readLevelData();    

    bool readLevelObjects();

private:

    bool m_ready;    
    string m_name;
    
    tilemap_t m_tilemap;    
    anitilemap_t m_seaTiles;
    anitilemap_t m_flowerTiles;
    anitilemap_t m_encounterTiles;

    npcs_t m_npcs;
    owobjects_t m_owobjects;

    std::shared_ptr<TextureResource> m_pLevelTex;      
    std::shared_ptr<TextureResource> m_pOverworldAtlas;

    SDL_Rect m_levelArea;
    uint32 m_rows, m_cols;
    int32 m_xOffset, m_yOffset;
};