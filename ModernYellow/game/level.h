/* ======================
   Date: 15/6/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once
#include "../portcommon.h"
#include "../mydef.h"

#include <vector>
#include <memory>

using std::string;
class Tile;
class TextureResource;
class Sprite;
class Level: public std::enable_shared_from_this<Level>
{    
public:
    using tilemap_t = std::vector<std::vector<std::shared_ptr<Tile>>>;
    
    explicit Level(
        const string& levelName, 
        const std::shared_ptr<TextureResource>& pAtlas);
    
    ~Level();

    void update() const;

    void render() const;

    inline bool isReady() const;

    void getNPCData(
        const std::shared_ptr<TextureResource>& pAtlas, 
        std::vector<std::unique_ptr<Sprite>>& outNpcs) const;

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

private:

    bool loadLevelTex();

    bool readLevelData();    

private:

    bool m_ready;
    string m_name;
    tilemap_t m_tilemap;    
    std::shared_ptr<TextureResource> m_levelTex;
    SDL_Rect m_levelArea;
    uint32 m_rows, m_cols;
    int32 m_xOffset, m_yOffset;
};