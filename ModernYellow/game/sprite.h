/* ======================
   Date: 16/6/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once
#include "../mydef.h"
#include <memory>

class TextureResource;
class Level;
class Tile;
class Sprite
{
public:

    enum SpriteState
    {
        S_MOVING, S_DIRFREEZE, S_IDLE
    };

public:

    static const uint32 SPRITE_MAX_FRAMES_PER_ANI   = 4U;
    static const uint32 SPRITE_MAX_CHANGE_DIR_DELAY = 4U;
    static const uint32 SPRITE_ANI_DELAY            = 8U;

public:

    Sprite(        
        const uint32 texU, 
        const uint32 texV,
        const std::shared_ptr<Tile> pInitTile,
        const std::shared_ptr<Level> pLevelRef,
        const std::shared_ptr<TextureResource>& pAtlas);

    virtual ~Sprite();
    
    virtual void tryMove(const Direction dir);

    virtual void tryChangeDirection(const Direction dir);

    virtual void update();

    virtual void render();
    
    const std::shared_ptr<Tile>& getCurrTile() const;

    const std::shared_ptr<Tile>& getNextTile() const;

    SpriteState getState() const;

    Direction getDir() const;

    uint32 getX() const;

    uint32 getY() const;

    void setWalkingAnimation(const bool walkingAnimation);
    
    void setState(const SpriteState state);

    void setOffset(
        const int32 xOffset = 0,
        const int32 yOffset = 0);

private:

    void updateAnimation();

    void updatePosition();

    void loadAnimations(
        const uint32 texU, 
        const uint32 texV,
        const std::shared_ptr<TextureResource>& pAtlas);

    void advanceUVs(
        uint32& texU,
        uint32& texV,
        const uint32 atlasWidth);

protected:

    struct spriteImpl;
    std::unique_ptr<spriteImpl> m_impl;


};