/* ======================
   Date: 16/6/2016
   Author: Alex Koukoulas
   ====================== */

#include "sprite.h"
#include "tile.h"
#include "level.h"
#include "../sinputhandler.h"
#include "../portcommon.h"
#include "../resources/textureresource.h"
#include <array>
#include <unordered_map>
#include <SDL_render.h>
#include <SDL_log.h>

extern uint32 g_tileSize;
extern SDL_Renderer* g_renderer;
extern uint32 g_scale;

/* Cached width of the last seen atlas texture */
static uint32 i_cachedAtlasWidth;


using frames_t = std::array<std::shared_ptr<TextureResource>, Sprite::SPRITE_MAX_FRAMES_PER_ANI>;
using animations_t = std::unordered_map<Direction, frames_t>;

struct Sprite::spriteImpl
{    
    animations_t m_anims;        
    SDL_Rect m_worldPos;
    Direction m_currDir;
    
    std::shared_ptr<Tile> m_pCurrTile;
    std::shared_ptr<Tile> m_pNextTile;
    std::shared_ptr<const Level> m_pLevelRef;

    uint32 m_xRendOffset, m_yRendOffset;    
    uint32 m_currFrame;
    uint32 m_frameTime;
    uint32 m_changeDirDelay;
    uint32 m_velocity;
    
    bool m_walkingAnimation;
    SpriteState m_currState;    
};

/* ==============
   Public Methods
   ============== */
Sprite::Sprite(    
    const uint32 texU,
    const uint32 texV,
    const std::shared_ptr<Tile> pInitTile,
    const std::shared_ptr<const Level> pLevelRef,
    const std::shared_ptr<TextureResource>& pAtlas):

    m_impl(std::make_unique<spriteImpl>())
{
    loadAnimations(texU, texV, pAtlas);

    m_impl->m_pLevelRef = pLevelRef;

    m_impl->m_pCurrTile = pInitTile;
    m_impl->m_pNextTile = pInitTile;

    m_impl->m_worldPos.x = m_impl->m_pCurrTile->getX();
    m_impl->m_worldPos.y = m_impl->m_pCurrTile->getY();
    m_impl->m_worldPos.w = g_tileSize;
    m_impl->m_worldPos.h = g_tileSize;

    m_impl->m_currDir = DIR_DOWN;

    m_impl->m_xRendOffset = 0;
    m_impl->m_yRendOffset = 0;
    m_impl->m_currFrame   = 0;
    m_impl->m_frameTime   = SPRITE_ANI_DELAY;    

    m_impl->m_changeDirDelay = SPRITE_MAX_CHANGE_DIR_DELAY;

    m_impl->m_velocity = g_tileSize / DEFAULT_TILE_SIZE;

    m_impl->m_currState = S_IDLE;

    m_impl->m_walkingAnimation = false;
}

Sprite::~Sprite()
{
}

void Sprite::tryMove(const Direction dir)
{    
    switch (m_impl->m_currState)
    {
        case S_MOVING:        
        case S_DIRFREEZE: break;

        case S_IDLE:
        {            
            std::shared_ptr<Tile> pNextTile;

            switch (dir)
            {
            case DIR_DOWN:  pNextTile = m_impl->m_pLevelRef->getTileBelow(m_impl->m_pCurrTile); break;
            case DIR_UP:    pNextTile = m_impl->m_pLevelRef->getTileAbove(m_impl->m_pCurrTile); break;
            case DIR_LEFT:  pNextTile = m_impl->m_pLevelRef->getTileLeftOf(m_impl->m_pCurrTile); break;
            case DIR_RIGHT: pNextTile = m_impl->m_pLevelRef->getTileRightOf(m_impl->m_pCurrTile); break;
            }

            if (pNextTile->getTileType() != TT_SOLID)
            {
                m_impl->m_currState = S_MOVING;
            }

            m_impl->m_currDir = dir;
            m_impl->m_pNextTile = pNextTile;
            m_impl->m_walkingAnimation = true;
        }
    }    
}

void Sprite::tryChangeDirection(const Direction dir)
{    
    switch (m_impl->m_currState)
    {
        case S_MOVING: break;

        case S_IDLE:
        case S_DIRFREEZE:
        {
            m_impl->m_currDir = dir;
            m_impl->m_currState = S_DIRFREEZE;
            m_impl->m_changeDirDelay = SPRITE_MAX_CHANGE_DIR_DELAY;
        } break;
    }        
}

void Sprite::update()
{   
    // Sprite movement works the following way: 
    // When a tryMove order is given, the sprite finds
    // its next tile (target tile) sets its state to MOVING 
    // (given that the next tile is not a solid one), and proceeds.
    // During the update position, the sprite pilot-moved into the next
    // tile, while during the animation update, the frames of the sprite are updated.
    // Animation is updated prior to position
    // To handle the case of the sprite having stepped on the
    // target tile, but the player keeps holding the movement button
    // (i.e. the movement will continue during the next frame). 
    // The animation resets when the sprite reaches its target tile
    // and in order to avoid the animation resetting mid-movement this
    // reordering must happen
    updateAnimation();
    updatePosition();        
}

void Sprite::updateAnimation()
{
    if (m_impl->m_walkingAnimation)
    {
        m_impl->m_frameTime--;
        if (m_impl->m_frameTime <= 0)
        {
            m_impl->m_frameTime = SPRITE_ANI_DELAY;
            m_impl->m_currFrame = (m_impl->m_currFrame + 1) % SPRITE_MAX_FRAMES_PER_ANI;
        }
    }
    else
    {
        m_impl->m_frameTime = SPRITE_ANI_DELAY;
        m_impl->m_currFrame = 0;
    }
}

void Sprite::updatePosition()
{
    switch (m_impl->m_currState)
    {
        case S_IDLE:
        {

        } break;

        case S_DIRFREEZE:
        {
            m_impl->m_changeDirDelay--;
            if (m_impl->m_changeDirDelay <= 0)
            {
                m_impl->m_changeDirDelay = 0;
                m_impl->m_currState = S_IDLE;
            }
        } break;

        case S_MOVING:
        {
            switch (m_impl->m_currDir)
            {
                case DIR_DOWN:  m_impl->m_worldPos.y += m_impl->m_velocity; break;
                case DIR_UP:    m_impl->m_worldPos.y -= m_impl->m_velocity; break;                
                case DIR_LEFT:  m_impl->m_worldPos.x -= m_impl->m_velocity; break;                
                case DIR_RIGHT: m_impl->m_worldPos.x += m_impl->m_velocity; break;                                
            }

            // Target tile reached
            if (
                m_impl->m_worldPos.x == m_impl->m_pNextTile->getX() &&
                m_impl->m_worldPos.y == m_impl->m_pNextTile->getY())
            {
                m_impl->m_pCurrTile = m_impl->m_pNextTile;
                m_impl->m_currState = S_IDLE;
                m_impl->m_walkingAnimation = false;
            }

        } break;
    }
}

void Sprite::render()
{
    SDL_Rect rendRect;
    rendRect.x = m_impl->m_worldPos.x + m_impl->m_xRendOffset;
    rendRect.y = m_impl->m_worldPos.y + m_impl->m_yRendOffset - 4 * g_scale;
    rendRect.w = g_tileSize;
    rendRect.h = g_tileSize;

    
    auto anim = m_impl->m_anims[m_impl->m_currDir];
    auto frame = anim[m_impl->m_currFrame];
    
    SDL_RenderCopy(
        g_renderer, 
        frame->getTexture().get(),
        nullptr, 
        &rendRect);
}

const std::shared_ptr<Tile>& Sprite::getCurrTile() const
{
    return m_impl->m_pCurrTile;
}

const std::shared_ptr<Tile>& Sprite::getNextTile() const
{
    return m_impl->m_pNextTile;
}

Sprite::SpriteState Sprite::getState() const
{
    return m_impl->m_currState;
}

Direction Sprite::getDir() const
{
    return m_impl->m_currDir;
}

uint32 Sprite::getX() const
{
    return m_impl->m_worldPos.x;
}

uint32 Sprite::getY() const
{
    return m_impl->m_worldPos.y;
}


void Sprite::setState(const SpriteState state)
{
    m_impl->m_currState = state;
}

void Sprite::setOffset(
    const int32 xOffset /* 0 */,
    const int32 yOffset /* 0 */)
{
    m_impl->m_xRendOffset = xOffset;
    m_impl->m_yRendOffset = yOffset;
}

void Sprite::setWalkingAnimation(const bool walkingAnimation)
{
    m_impl->m_walkingAnimation = walkingAnimation;
}

/* ===============
   Private Methods
   =============== */
void Sprite::loadAnimations(
    const uint32 texU, 
    const uint32 texV,
    const std::shared_ptr<TextureResource>& pAtlas)
{
    // Save the num. of cols per row in the atlas texture
    i_cachedAtlasWidth = pAtlas->getSurface()->w;

    uint32 u = texU;
    uint32 v = texV;

    // Load #1 Animations
    m_impl->m_anims[DIR_DOWN][0]  = pAtlas->getSubTexture(u, v, DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE); advanceUVs(u, v, i_cachedAtlasWidth);    
    m_impl->m_anims[DIR_UP][0]    = pAtlas->getSubTexture(u, v, DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE); advanceUVs(u, v, i_cachedAtlasWidth);
    m_impl->m_anims[DIR_LEFT][0]  = pAtlas->getSubTexture(u, v, DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE); advanceUVs(u, v, i_cachedAtlasWidth);
    m_impl->m_anims[DIR_RIGHT][0] = m_impl->m_anims[DIR_LEFT][0]->getHorFlippedTexture();

    // Load #2 Animations
    m_impl->m_anims[DIR_DOWN][1]  = pAtlas->getSubTexture(u, v, DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE); advanceUVs(u, v, i_cachedAtlasWidth);
    m_impl->m_anims[DIR_UP][1]    = pAtlas->getSubTexture(u, v, DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE); advanceUVs(u, v, i_cachedAtlasWidth);
    m_impl->m_anims[DIR_LEFT][1]  = pAtlas->getSubTexture(u, v, DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE); advanceUVs(u, v, i_cachedAtlasWidth);
    m_impl->m_anims[DIR_RIGHT][1] = m_impl->m_anims[DIR_LEFT][1]->getHorFlippedTexture();

    // Load #3 Animations
    m_impl->m_anims[DIR_DOWN][2]  = m_impl->m_anims[DIR_DOWN][0];
    m_impl->m_anims[DIR_UP][2]    = m_impl->m_anims[DIR_UP][0];
    m_impl->m_anims[DIR_LEFT][2]  = m_impl->m_anims[DIR_LEFT][0];
    m_impl->m_anims[DIR_RIGHT][2] = m_impl->m_anims[DIR_RIGHT][0];
    
    // Load #3 Animations
    m_impl->m_anims[DIR_DOWN][3]  = m_impl->m_anims[DIR_DOWN][1]->getHorFlippedTexture();
    m_impl->m_anims[DIR_UP][3]    = m_impl->m_anims[DIR_UP][1]->getHorFlippedTexture();
    m_impl->m_anims[DIR_LEFT][3]  = m_impl->m_anims[DIR_LEFT][1];
    m_impl->m_anims[DIR_RIGHT][3] = m_impl->m_anims[DIR_RIGHT][1];
}

void Sprite::advanceUVs(
    uint32& texU,
    uint32& texV,
    const uint32 atlasWidth)
{
    texU += DEFAULT_TILE_SIZE;
    if (texU == atlasWidth)
    {
        texU = 0;
        texV += DEFAULT_TILE_SIZE;
    }
}
