/* ======================
   Date: 22/6/2016
   Author: Alex Koukoulas
   ====================== */

#include "overworldobject.h"
#include "tile.h"
#include "../resources/textureresource.h"

#include <SDL_render.h>

extern uint32 g_scale;
extern uint32 g_tileSize;
extern uint32 g_overworldTilemapColor;
extern pRenderer_t g_pRenderer;

OWObject::OWObject(
    const int32 tu,
    const int32 tv,
    const std::shared_ptr<TextureResource> pAtlas,
    const std::shared_ptr<Tile> pInitTile,
    const std::string& dialogue,
    const std::string& dialogueAfterInteraction,
    const bool obtainable,
    const bool solid,
    const bool cuttable):

    m_pTile(pInitTile),
    m_pTex(nullptr),
    m_dialogue(std::move(dialogue)),
    m_dialogueAfterInteraction(std::move(dialogueAfterInteraction)),
    m_xOffset(0),
    m_yOffset(0)
{
    if (obtainable) m_flags |= OWO_FLAG_OBTAINABLE;
    if (solid)      m_flags |= OWO_FLAG_SOLID;
    if (cuttable)   m_flags |= OWO_FLAG_CUTTABLE;

    if (tu != -1 && tv != -1)
    {
        m_pTex = pAtlas->getSubTexture(tu, tv, DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE);
    }    

    if (solid)
    {
        pInitTile->setOccupied(true);
    }
}

void OWObject::update()
{

}

void OWObject::render()
{
    if (m_pTex)
    {
        SDL_Rect rendArea;
        rendArea.x = m_pTile->getX() + m_xOffset;
        rendArea.y = m_pTile->getY() + m_yOffset;
        rendArea.w = g_tileSize;
        rendArea.h = g_tileSize;

        SDL_RenderCopy(g_pRenderer.get(), m_pTex->getTexture().get(), nullptr, &rendArea);
    }    
}

void OWObject::switchPaletteTo(const uint32 color)
{
    m_pTex->swapColor(g_overworldTilemapColor, color);
}

void OWObject::darken()
{
    m_pTex->darken();
}

void OWObject::switchDialogue()
{
    m_dialogue = m_dialogueAfterInteraction;
}

const std::string& OWObject::getDialogue() const
{
    return m_dialogue;
}

std::shared_ptr<Tile> OWObject::getCurrTile() const
{
    return m_pTile;
}

void OWObject::setOffset(int32 xOffset /* 0 */, int32 yOffset /* 0 */)
{
    m_xOffset = xOffset;
    m_yOffset = yOffset;
}
