/* ======================
   Date: 15/6/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once
#include "../mydef.h"

class Sprite;
class Tile;
class Level;
class TextureResource;
class AudioResource;
class UIComponent;
class Player final
{    
public:

    Player(
        const std::shared_ptr<Tile> pInitTile,
        const Direction initDir,
        const std::shared_ptr<Level> pLevelRef,
        const std::shared_ptr<TextureResource>& pAtlas);

    ~Player();

    void update();
    
    void render();

    bool hasUIDialog() const;

    std::shared_ptr<Tile> getCurrTile() const;

    std::shared_ptr<Tile> getNextTile() const;

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
    struct WarpInfo;

    std::shared_ptr<Level> m_pLevelRef;
    std::unique_ptr<Sprite> m_pSprite;
    std::unique_ptr<UIComponent> m_pTextbox;    
    std::unique_ptr<WarpInfo> m_warpInfo;    
    bool m_firstTileAfterWarp;
    bool m_standingAtDoor;
    bool m_switchingAmbient;

};