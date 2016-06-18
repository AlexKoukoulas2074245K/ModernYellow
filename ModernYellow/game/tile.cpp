/* ======================
   Date: 15/6/2016
   Author: Alex Koukoulas
   ====================== */

#include "tile.h"
#include <SDL_log.h>

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

bool Tile::operator == (const Tile& rhs)
{
    SDL_Log("Called operator ==");
    return getCol() == rhs.getCol() && 
           getRow() == rhs.getRow();
}

TileType Tile::getTileType() const
{
    return m_tileType;
}

int32 Tile::getX() const
{
    return m_col * g_tileSize;
}

int32 Tile::getY() const
{
    return m_row * g_tileSize;
}

uint32 Tile::getRow() const
{
    return m_row;
}

uint32 Tile::getCol() const
{
    return m_col;
}