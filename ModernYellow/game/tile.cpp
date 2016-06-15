/* ======================
   Date: 15/6/2016
   Author: Alex Koukoulas
   ====================== */

#include "tile.h"

extern uint32 g_scale;

Tile::Tile(const uint32 col,
           const uint32 row,           
           const uint32 tileType):
    
    m_col(col),
    m_row(row),    
    m_tileType((TileType)tileType),
    m_tileSize(DEFAULT_TILE_SIZE * g_scale)
{    
}

Tile::~Tile() = default;

TileType Tile::getTileType() const
{
    return m_tileType;
}

SDL_Rect Tile::getTileRect() const
{
    SDL_Rect rect;
    rect.x = m_col * m_tileSize;
    rect.y = m_row * m_tileSize;
    rect.w = m_tileSize;
    rect.h = m_tileSize;
    return rect;
}