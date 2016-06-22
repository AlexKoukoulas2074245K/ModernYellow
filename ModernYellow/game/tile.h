/* ======================
   Date: 15/6/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once
#include "../portcommon.h"
#include "../mydef.h"

#include <vector>

class TextureResource;
struct  Warp;
class Tile final
{
public:

    static const uint16 TT_FREE      = 0 << 0;
    static const uint16 TT_SOLID     = 1 << 0;
    static const uint16 TT_ENCOUNTER = 1 << 1;
    static const uint16 TT_SEA       = 1 << 2;
    static const uint16 TT_LEDGE     = 1 << 3;
    static const uint16 TT_FLOWER_1  = 1 << 4;
    static const uint16 TT_FLOWER_2  = 1 << 5;

public:
        
    explicit Tile(const uint32 col,
                  const uint32 row,
                  const uint16 tileType);

    ~Tile();

    void update();

    void render(const int32 xRendOffset, const int32 yRendOffset);

    std::shared_ptr<Warp> getWarp() const;

    uint16 getTileType() const;
    
    int32 getX() const;
    
    int32 getY() const;
    
    uint32 getRow() const;

    uint32 getCol() const;


    bool isWalkable() const;
    
    bool isOccupied() const;

    bool hasSeaTex() const;
  
    void addWarp(std::shared_ptr<Warp> warp);

    void setOccupied(const bool occupied);

    void addSeaTile();

    void darken();

private:

    void addFlowerTile1();
    
    void addFlowerTile2();

    void addGrassTile();

private:

    static const uint32 TILE_FLOWER_ANI_DELAY  = 40U;
    static const uint32 TILE_SEA_ANI_DELAY     = 20U;
    static const uint32 TILE_FLOWER_MAX_FRAMES = 3U;
    static const uint32 TILE_SEA_MAX_FRAMES    = 1U;

private:
    
    uint16 m_tileType;
    uint32 m_col;
    uint32 m_row;    
    bool m_occupied;      
    std::vector<std::shared_ptr<TextureResource>> m_frames;
    std::shared_ptr<Warp> m_warp;
    bool m_seaCurrentLeft;
    uint32 m_seaCurrentDelay;
    int32  m_seaCurrentOffset;
    uint32 m_curFrame;
    uint32 m_flowerAniDelay;
};