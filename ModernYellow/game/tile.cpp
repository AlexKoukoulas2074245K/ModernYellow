/* ======================
   Date: 15/6/2016
   Author: Alex Koukoulas
   ====================== */

#include "tile.h"

extern uint32 g_tileSize;

Tile::Tile(
    const uint32 col,
    const uint32 row,           
    const uint32 tileType):
    
    m_col(col),
    m_row(row),    
    m_tileType((TileType)tileType)    
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
    rect.x = m_col * g_tileSize;
    rect.y = m_row * g_tileSize;
    rect.w = g_tileSize;
    rect.h = g_tileSize;
    return rect;
}