/* ======================
   Date: 15/6/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once
#include "../mydef.h"
#include <memory>

class Sprite;
class Tile;
class Level;
class TextureResource;
class Player final
{
public:

    Player(
        const std::shared_ptr<Tile> pInitTile,
        const Direction initDir,
        const std::shared_ptr<const Level> pLevelRef,
        const std::shared_ptr<TextureResource>& pAtlas);

    ~Player();

    void update();

    void render();

    int32 getX() const;

    int32 getY() const;
    
    void setOffset(
        const int32 x = 0,
        const int32 y = 0);

private:

    void cancelWalkingAnimation();

private:

    static const uint32 PLAYER_TEX_U = 6  * DEFAULT_TILE_SIZE;
    static const uint32 PLAYER_TEX_V = 14 * DEFAULT_TILE_SIZE;

private:

    std::shared_ptr<const Level> m_pLevelRef;
    std::unique_ptr<Sprite> m_pSprite;

};