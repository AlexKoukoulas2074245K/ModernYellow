/* ======================
   Date: 15/6/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once
#include "../portcommon.h"
#include "../mydef.h"

enum TileType
{
    TT_FREE, 
    TT_SOLID, 
    TT_ENCOUNTER, 
    TT_WATER, 
    TT_LEDGE, 
    TT_FLOWER_1, 
    TT_FLOWER_2
};

class Tile final
{
public:
    
    explicit Tile(const uint32 col,
                  const uint32 row,
                  const uint32 tileType);

    ~Tile();

    bool operator == (const Tile& rhs);

    TileType getTileType() const;
    
    int32 getX() const;
    
    int32 getY() const;
    
    uint32 getRow() const;

    uint32 getCol() const;

private:
    
    TileType m_tileType;
    uint32 m_col;
    uint32 m_row;    
};