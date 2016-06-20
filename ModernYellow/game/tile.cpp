/* ======================
   Date: 15/6/2016
   Author: Alex Koukoulas
   ====================== */

#include "tile.h"
#include <SDL_log.h>
#include <SDL_render.h>

#include "../resources/textureresource.h"
#include "../resources/sresmanager.h"

extern uint32 g_tileSize;
extern pRenderer_t g_pRenderer;

/* ==============
   Public Methods
   ============== */
Tile::Tile(
    const uint32 col,
    const uint32 row,           
    const uint16 tileType):
    
    m_col(col),
    m_row(row),    
    m_tileType(tileType),
    m_occupied(false),    
    m_seaCurrentLeft(true),
    m_seaCurrentOffset(0),    
    m_curFrame(0),
    m_flowerAniDelay(TILE_FLOWER_ANI_DELAY),
    m_seaCurrentDelay(TILE_SEA_ANI_DELAY)
{    
    if (m_tileType == TT_FLOWER_1)       addFlowerTile1();
    else if (m_tileType == TT_FLOWER_2)  addFlowerTile2();    
    else if (m_tileType == TT_SEA)       addSeaTile();
    else if (m_tileType == TT_ENCOUNTER) addGrassTile();
}

Tile::~Tile() = default;

void Tile::update()
{
    if (m_frames.size() <= 0) return;

    if (m_tileType == TT_FLOWER_1 || m_tileType == TT_FLOWER_2)
    {        
        if (m_flowerAniDelay-- <= 0) 
        {
            m_flowerAniDelay = TILE_FLOWER_ANI_DELAY;
            m_curFrame = (m_curFrame + 1) % TILE_FLOWER_MAX_FRAMES;
        }        
    }
    else if (m_frames.size() == TILE_SEA_MAX_FRAMES)
    {        
        if (m_seaCurrentDelay-- <= 0)
        {
            m_seaCurrentDelay = TILE_SEA_ANI_DELAY;
            if (m_seaCurrentLeft)
            {                
                if (m_seaCurrentOffset-- <= -4) m_seaCurrentLeft = false;
            }
            else
            {
                if (m_seaCurrentOffset++ >= 4) m_seaCurrentLeft = true;
            }
        }        
    }    
}

void Tile::render(const int32 xRendOffset, const int32 yRendOffset)
{
    if (m_frames.size() <= 0) return;

    SDL_Rect rendArea = {};
    rendArea.x = getX() + xRendOffset + (m_seaCurrentOffset*2);
    rendArea.y = getY() + yRendOffset;
    rendArea.w = g_tileSize;
    rendArea.h = g_tileSize;

    SDL_RenderCopy(
        g_pRenderer.get(),
        m_frames[m_curFrame]->getTexture().get(),
        nullptr, 
        &rendArea);
}

uint16 Tile::getTileType() const
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

bool Tile::isWalkable() const
{
    return !m_occupied && m_tileType != TT_SOLID;
}

bool Tile::isOccupied() const
{
    return m_occupied;
}

bool Tile::hasSeaTex() const
{
    return m_frames.size() == TILE_SEA_MAX_FRAMES;
}

void Tile::setOccupied(const bool occupied)
{
    m_occupied = occupied;
}

void Tile::addSeaTile()
{    
    m_frames.push_back(castResToTex(resmanager.loadResource("misctex/seaTile.png", RT_TEXTURE)));
}

void Tile::addGrassTile()
{
    m_frames.push_back(castResToTex(resmanager.loadResource("misctex/grassTile.png", RT_TEXTURE)));
}

/* ===============
   Private Methods
   =============== */
void Tile::addFlowerTile1()
{
    m_frames.push_back(castResToTex(resmanager.loadResource("misctex/flowerTile11.png", RT_TEXTURE)));
    m_frames.push_back(castResToTex(resmanager.loadResource("misctex/flowerTile12.png", RT_TEXTURE)));
    m_frames.push_back(castResToTex(resmanager.loadResource("misctex/flowerTile13.png", RT_TEXTURE)));
}

void Tile::addFlowerTile2()
{
    m_frames.push_back(castResToTex(resmanager.loadResource("misctex/flowerTile21.png", RT_TEXTURE)));
    m_frames.push_back(castResToTex(resmanager.loadResource("misctex/flowerTile22.png", RT_TEXTURE)));
    m_frames.push_back(castResToTex(resmanager.loadResource("misctex/flowerTile23.png", RT_TEXTURE)));
}
