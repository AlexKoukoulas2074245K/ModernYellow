/* ======================
   Date: 16/6/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once
#include "../mydef.h"
#include <memory>

class TextureResource;
class Sprite
{
public:

    static const uint32 SPRITE_MAX_FRAMES_PER_ANI = 4U;

public:

    Sprite(
        const int32 worldX,
        const int32 worldY,
        const uint32 texU, 
        const uint32 texV,
        const std::shared_ptr<TextureResource>& pAtlas);

    virtual ~Sprite();
    
    virtual void update();

    virtual void render();

    void setOffset(
        const int32 xOffset = 0,
        const int32 yOffset = 0);
    

    uint32 getX() const;
    uint32 getY() const;

private:

    void loadAnimations(
        const uint32 texU, 
        const uint32 texV,
        const std::shared_ptr<TextureResource>& pAtlas);

    void advanceUVs(
        uint32& texU,
        uint32& texV,
        const uint32 atlasWidth);

private:

    struct spriteImpl;
    std::unique_ptr<spriteImpl> m_impl;

};