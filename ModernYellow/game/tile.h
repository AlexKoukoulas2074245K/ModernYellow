/* ======================
   Date: 15/6/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once
#include "../portcommon.h"
#include "../mydef.h"

enum TileType
{
    FREE, SOLID, ENCOUNTER, WATER, LEDGE, FLOWER_1, FLOWER_2
};

class Tile final
{
public:
    
    explicit Tile(const uint32 col,
                  const uint32 row,
                  const uint32 tileType);

    ~Tile();

    TileType getTileType() const;
    
    SDL_Rect getTileRect() const;

private:
    
    TileType m_tileType;
    uint32 m_col;
    uint32 m_row;    
};