/* ======================
   Date: 22/6/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once
#include "../mydef.h"

#include <string>

class Tile;
class TextureResource;
class OWObject final
{
public:

    OWObject(
        const int32 tu,
        const int32 tv,
        const std::shared_ptr<TextureResource> pAtlas,
        const std::shared_ptr<Tile> pInitTile,
        const std::string& dialogue,
        const bool obtainable,
        const bool solid,
        const bool cuttable);
    
    void update();

    void render();

    const std::string& getDialogue() const;
    
    std::shared_ptr<Tile> getCurrTile() const;

    void setOffset(int32 xOffset = 0, int32 yOffset = 0);    

private:

    static const uint32 OWO_FLAG_OBTAINABLE = 1 << 0;
    static const uint32 OWO_FLAG_SOLID      = 1 << 1;
    static const uint32 OWO_FLAG_CUTTABLE   = 1 << 2;

private:

    uint32 m_flags;
    std::shared_ptr<TextureResource> m_pTex;
    std::shared_ptr<Tile> m_pTile;
    std::string m_dialogue;
    int32 m_xOffset, m_yOffset;
};