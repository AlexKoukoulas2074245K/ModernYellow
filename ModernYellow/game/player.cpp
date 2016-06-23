/* ======================
   Date: 15/6/2016
   Author: Alex Koukoulas
   ====================== */

#include "player.h"
#include "level.h"
#include "sprite.h"
#include "npc.h"
#include "overworldobject.h"
#include "tile.h"
#include "uicomps/uitextbox.h"
#include "../resources/sresmanager.h"
#include "../resources/textureresource.h"
#include "../sinputhandler.h"
#include "../font.h"

#include <SDL_log.h>

extern pFont_t g_pFont;

/* ==============
   Public Methods
   ============== */
Player::Player(
    const std::shared_ptr<Tile> pInitTile,
    const Direction initDir,
    const std::shared_ptr<Level> pLevelRef,
    const std::shared_ptr<TextureResource>& pAtlas):

    m_pLevelRef(pLevelRef),
    m_firstTileAfterWarp(false),
    m_warpAfterCol(0),
    m_warpAfterRow(0),    
    m_warping(false),
    m_warpTimer(Level::LEVEL_WARP_LEVEL_DELAY),
    m_standingAtDoor(false)
{
    m_pSprite = std::make_unique<Sprite>(
        PLAYER_TEX_U,
        PLAYER_TEX_V,
        pInitTile,
        initDir,
        m_pLevelRef,
        pAtlas);
}

Player::~Player(){}

void Player::update()
{
    // The has UI components active and 
    // will only update them 
    if (hasUIDialog())
    {
        m_pTextbox->update();

        if (m_pTextbox->isFinished())
        {
            m_pTextbox = nullptr;
        }
        return;
    }
    
    // The player is in the middle of a warp to 
    // another location, and is gradually performing
    // the move along with the level
    if (m_warping)
    {        
        if (--m_warpTimer <= 0)
        {
            m_warpTimer = Level::LEVEL_WARP_LEVEL_DELAY;
            m_pSprite->darken();
        }

        // Global darken animation has finished
        if (m_pLevelRef->getWarpLevel() == Level::LEVEL_WARP_LEVEL_MAX)
        {           
            // Reload any resources as needed
            m_pSprite->reloadFrames(
                PLAYER_TEX_U, 
                PLAYER_TEX_V, 
                castResToTex(resmanager.loadResource("tilemaps/overworldmap.png", RT_TEXTURE)));
            m_pSprite->teleportTo(m_pLevelRef->getTileRC(m_warpAfterCol, m_warpAfterRow));

            // If the player moved from an inside location to an outside one, the player
            // should start in moving state targeting the tile below the door of the building
            if (m_standingAtDoor)
            {
                m_pSprite->setState(Sprite::S_IDLE);
                m_pSprite->tryMove(DIR_DOWN);
            }
            
            // Signal level to reset the warp related variables
            m_firstTileAfterWarp = true;
            m_warping = false;
            m_standingAtDoor = false;
            m_pLevelRef->resetWarping();
        }
        
        return;
    }
        
    // A button tap, need to test for a ui or chat action
    if (ihandler.isKeyTapped(K_A))
    {        
        switch (m_pSprite->getDir())
        {
            case DIR_DOWN:  
            {
                auto pNpc = m_pLevelRef->getNpcAt(m_pLevelRef->getTileBelow(m_pSprite->getCurrTile())); 
                auto pOwo = m_pLevelRef->getOWObjectAt(m_pLevelRef->getTileBelow(m_pSprite->getCurrTile()));

                if (pNpc)
                {
                    pNpc->tryChangeDirection(DIR_UP);
                    m_pTextbox = std::make_unique<UITextbox>(pNpc->getDialogue());                    
                }
                else if (pOwo)
                {                    
                    m_pTextbox = std::make_unique<UITextbox>(pOwo->getDialogue());
                }

            } break;

            case DIR_UP:    
            {
                auto pNpc = m_pLevelRef->getNpcAt(m_pLevelRef->getTileAbove(m_pSprite->getCurrTile()));
                auto pOwo = m_pLevelRef->getOWObjectAt(m_pLevelRef->getTileAbove(m_pSprite->getCurrTile()));

                if (pNpc)
                {
                    pNpc->tryChangeDirection(DIR_DOWN);
                    m_pTextbox = std::make_unique<UITextbox>(pNpc->getDialogue());
                }
                else if (pOwo)
                {
                    m_pTextbox = std::make_unique<UITextbox>(pOwo->getDialogue());
                }
            } break;

            case DIR_LEFT:  
            {
                auto pNpc = m_pLevelRef->getNpcAt(m_pLevelRef->getTileLeftOf(m_pSprite->getCurrTile())); 
                auto pOwo = m_pLevelRef->getOWObjectAt(m_pLevelRef->getTileLeftOf(m_pSprite->getCurrTile()));

                if (pNpc)
                {
                    pNpc->tryChangeDirection(DIR_RIGHT);
                    m_pTextbox = std::make_unique<UITextbox>(pNpc->getDialogue());
                }
                else if (pOwo)
                {
                    m_pTextbox = std::make_unique<UITextbox>(pOwo->getDialogue());
                }
            } break;

            case DIR_RIGHT: 
            {
                auto pNpc = m_pLevelRef->getNpcAt(m_pLevelRef->getTileRightOf(m_pSprite->getCurrTile())); 
                auto pOwo = m_pLevelRef->getOWObjectAt(m_pLevelRef->getTileRightOf(m_pSprite->getCurrTile()));

                if (pNpc)
                {
                    pNpc->tryChangeDirection(DIR_LEFT);
                    m_pTextbox = std::make_unique<UITextbox>(pNpc->getDialogue());
                }
                else if (pOwo)
                {
                    m_pTextbox = std::make_unique<UITextbox>(pOwo->getDialogue());
                }
            } break;
        }
    }

    // Directional key taps. This will either
    // result in a TryMove or a TryChangeDirection
    // operations (and possible a warp only for DIR_DOWN)
    if (ihandler.isKeyTapped(K_DOWN)) 
    {
        if (m_pSprite->getDir() == DIR_DOWN)
        {
            if (m_standingAtDoor)
            {
                auto pWarp = m_pSprite->getCurrTile()->getWarp();
                m_warping = true;
                m_warpAfterDir = pWarp->forcedDir == -1 ? m_pSprite->getDir() : (Direction) pWarp->forcedDir;
                m_warpAfterCol = pWarp->col;
                m_warpAfterRow = pWarp->row;
                m_pLevelRef->startWarpTo(pWarp);
            }
            else
            {
                m_pSprite->tryMove(DIR_DOWN);
            }            
        }
        else
        {
            m_pSprite->tryChangeDirection(DIR_DOWN);
        }        
    }
    else if (ihandler.isKeyTapped(K_UP))    m_pSprite->getDir() == DIR_UP    ? m_pSprite->tryMove(DIR_UP)    : m_pSprite->tryChangeDirection(DIR_UP);
    else if (ihandler.isKeyTapped(K_LEFT))  m_pSprite->getDir() == DIR_LEFT  ? m_pSprite->tryMove(DIR_LEFT)  : m_pSprite->tryChangeDirection(DIR_LEFT);
    else if (ihandler.isKeyTapped(K_RIGHT)) m_pSprite->getDir() == DIR_RIGHT ? m_pSprite->tryMove(DIR_RIGHT) : m_pSprite->tryChangeDirection(DIR_RIGHT);
    
    // Directional key downs. This will always result
    // in a TryMove operation (or possibly a warp for DIR_DOWN)
    else if (ihandler.isKeyDown(K_DOWN))    
    {
        if (m_standingAtDoor)
        {
            auto pWarp = m_pSprite->getCurrTile()->getWarp();
            m_warping = true;
            m_warpAfterDir = pWarp->forcedDir == -1 ? m_pSprite->getDir() : (Direction) pWarp->forcedDir;
            m_warpAfterCol = pWarp->col;
            m_warpAfterRow = pWarp->row;
            m_pLevelRef->startWarpTo(pWarp);
        }
        else
        {
            m_pSprite->tryMove(DIR_DOWN);
        }        
    }
    else if (ihandler.isKeyDown(K_UP))      m_pSprite->tryMove(DIR_UP);    
    else if (ihandler.isKeyDown(K_LEFT))    m_pSprite->tryMove(DIR_LEFT);    
    else if (ihandler.isKeyDown(K_RIGHT))   m_pSprite->tryMove(DIR_RIGHT);    

    // No related key downs. Need to test for cancelling the 
    // walking animation if the player is trying to walk against a solid tile
    else
    {
        if (!m_pSprite->getNextTile()->isWalkable() && 
            m_pSprite->getState() != Sprite::S_MOVING)
        {            
            if ((m_pSprite->getDir() == DIR_DOWN  && !ihandler.isKeyDown(K_DOWN))   ||
                (m_pSprite->getDir() == DIR_UP    && !ihandler.isKeyDown(K_UP))   ||
                (m_pSprite->getDir() == DIR_LEFT  && !ihandler.isKeyDown(K_LEFT)) ||
                (m_pSprite->getDir() == DIR_RIGHT && !ihandler.isKeyDown(K_RIGHT)))
            {                
                m_pSprite->setState(Sprite::S_IDLE);
                m_pSprite->setWalkingAnimation(false);
            }
        }
    }

    auto prevTile = m_pSprite->getCurrTile();
    m_pSprite->update();
    auto currTile = m_pSprite->getCurrTile();

    m_standingAtDoor = m_pLevelRef->isInnerDoor(currTile);

    // The player has just completed a move and 
    // is residing on a new tile
    if (prevTile != currTile)
    {        
        m_firstTileAfterWarp = false;
        auto pWarp = currTile->getWarp();
        if (currTile->getWarp() && 
            !m_firstTileAfterWarp && 
            !m_standingAtDoor)
        {               
            m_warping = true;
            m_warpAfterDir = pWarp->forcedDir == -1 ? m_pSprite->getDir() : (Direction) pWarp->forcedDir;
            m_warpAfterCol = pWarp->col;
            m_warpAfterRow = pWarp->row;
            m_pLevelRef->startWarpTo(pWarp);
        }        
    }    
}

void Player::render()
{
    m_pSprite->render();

    if (hasUIDialog())
    {
        m_pTextbox->render();
    }
}

bool Player::hasUIDialog() const
{
    return m_pTextbox && !m_pTextbox->isFinished();
}

std::shared_ptr<Tile> Player::getCurrTile() const
{
    return m_pSprite->getCurrTile();
}

std::shared_ptr<Tile> Player::getNextTile() const
{
    return m_pSprite->getNextTile();
}

void Player::setOffset(
    const int32 x /* 0 */,
    const int32 y /* 0 */)
{
    m_pSprite->setOffset(x, y);
}

int32 Player::getX() const
{
    return m_pSprite->getX();
}

int32 Player::getY() const
{
    return m_pSprite->getY();
}