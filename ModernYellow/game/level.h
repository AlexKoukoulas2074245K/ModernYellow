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
class Level final
{    
public:
    using tilemap_t = std::vector<std::vector<std::unique_ptr<Tile>>>;

    explicit Level(const string& levelName);
    
    ~Level();

    void render() const;

    bool isReady() const;

private:

    bool readLevelData();

    bool loadLevelTex();

private:

    bool m_ready;
    string m_name;
    tilemap_t m_tilemap;
    std::shared_ptr<TextureResource> m_levelTex;
    SDL_Rect m_levelArea;
    uint32 m_rows, m_cols;
};