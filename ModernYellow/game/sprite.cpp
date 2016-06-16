/* ======================
   Date: 16/6/2016
   Author: Alex Koukoulas
   ====================== */

#include "sprite.h"
#include "../sinputhandler.h"
#include "../portcommon.h"
#include "../resources/textureresource.h"
#include <array>
#include <unordered_map>
#include <SDL_render.h>


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
    uint32 m_xRendOffset, m_yRendOffset;
    Direction m_currDir;
    uint32 m_currFrame;
};

/* ==============
   Public Methods
   ============== */
Sprite::Sprite(
    const int32 worldX,
    const int32 worldY,
    const uint32 texU,
    const uint32 texV,
    const std::shared_ptr<TextureResource>& pAtlas):

    m_impl(std::make_unique<spriteImpl>())    
{
    loadAnimations(texU, texV, pAtlas);

    m_impl->m_worldPos.x = worldX;
    m_impl->m_worldPos.y = worldY;
    m_impl->m_worldPos.w = g_tileSize;
    m_impl->m_worldPos.h = g_tileSize;

    m_impl->m_currDir = DIR_DOWN;

    m_impl->m_xRendOffset = 0;
    m_impl->m_yRendOffset = 0;
    m_impl->m_currFrame = 0;
}

Sprite::~Sprite()
{
}

void Sprite::update()
{
    if (ihandler.isKeyDown(K_LEFT))       {m_impl->m_worldPos.x -= 1; m_impl->m_currDir = DIR_LEFT;}
    else if (ihandler.isKeyDown(K_RIGHT)) {m_impl->m_worldPos.x += 1; m_impl->m_currDir = DIR_RIGHT;}
    else if (ihandler.isKeyDown(K_UP))    {m_impl->m_worldPos.y -= 1; m_impl->m_currDir = DIR_UP;}
    else if (ihandler.isKeyDown(K_DOWN))  {m_impl->m_worldPos.y += 1; m_impl->m_currDir = DIR_DOWN;}
}

void Sprite::render()
{
    SDL_Rect rendRect;
    rendRect.x = m_impl->m_worldPos.x + m_impl->m_xRendOffset;
    rendRect.y = m_impl->m_worldPos.y + m_impl->m_yRendOffset + 4 * g_scale;
    rendRect.w = g_tileSize;
    rendRect.h = g_tileSize;

    auto currAnim = m_impl->m_anims[m_impl->m_currDir];
    auto currFrame = currAnim[m_impl->m_currFrame];

    SDL_RenderCopy(
        g_renderer, 
        currFrame->getTexture().get(),
        nullptr, 
        &rendRect);
}

void Sprite::setOffset(
    const int32 xOffset /* 0 */,
    const int32 yOffset /* 0 */)
{
    m_impl->m_xRendOffset = xOffset;
    m_impl->m_yRendOffset = yOffset;
}

uint32 Sprite::getX() const
{
    return m_impl->m_worldPos.x;
}

uint32 Sprite::getY() const
{
    return m_impl->m_worldPos.y;
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

    // Load First Animations
    m_impl->m_anims[DIR_DOWN][0]  = pAtlas->getSubTexture(u, v, DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE); advanceUVs(u, v, i_cachedAtlasWidth);    
    m_impl->m_anims[DIR_UP][0]    = pAtlas->getSubTexture(u, v, DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE); advanceUVs(u, v, i_cachedAtlasWidth);
    m_impl->m_anims[DIR_LEFT][0]  = pAtlas->getSubTexture(u, v, DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE); advanceUVs(u, v, i_cachedAtlasWidth);
    m_impl->m_anims[DIR_RIGHT][0] = m_impl->m_anims[DIR_LEFT][0]->getHorFlippedTexture();

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
