/* ======================
   Date: 15/6/2016
   Author: Alex Koukoulas
   ====================== */

#include "player.h"
#include "level.h"
#include "sprite.h"
#include "npc.h"
#include "tile.h"
#include "../sinputhandler.h"
#include <SDL_log.h>

/* ==============
   Public Methods
   ============== */
Player::Player(
    const std::shared_ptr<Tile> pInitTile,
    const Direction initDir,
    const std::shared_ptr<const Level> pLevelRef,
    const std::shared_ptr<TextureResource>& pAtlas):

    m_pLevelRef(pLevelRef)
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
    if (ihandler.isKeyTapped(K_A))
    {
        std::shared_ptr<Npc> pNpc;
        switch (m_pSprite->getDir())
        {
            case DIR_DOWN: pNpc = m_pLevelRef->getNpcAt(m_pLevelRef->getTileBelow(m_pSprite->getCurrTile())); break;
            case DIR_UP: pNpc = m_pLevelRef->getNpcAt(m_pLevelRef->getTileAbove(m_pSprite->getCurrTile())); break;
            case DIR_LEFT: pNpc = m_pLevelRef->getNpcAt(m_pLevelRef->getTileLeftOf(m_pSprite->getCurrTile())); break;
            case DIR_RIGHT: pNpc = m_pLevelRef->getNpcAt(m_pLevelRef->getTileRightOf(m_pSprite->getCurrTile())); break;
        }
        
        if (pNpc)
        {
            SDL_Log(pNpc->getDialogue().c_str());
        }
    }

    if      (ihandler.isKeyTapped(K_DOWN))  m_pSprite->getDir() == DIR_DOWN  ? m_pSprite->tryMove(DIR_DOWN)  : m_pSprite->tryChangeDirection(DIR_DOWN);    
    else if (ihandler.isKeyTapped(K_UP))    m_pSprite->getDir() == DIR_UP    ? m_pSprite->tryMove(DIR_UP)    : m_pSprite->tryChangeDirection(DIR_UP);    
    else if (ihandler.isKeyTapped(K_LEFT))  m_pSprite->getDir() == DIR_LEFT  ? m_pSprite->tryMove(DIR_LEFT)  : m_pSprite->tryChangeDirection(DIR_LEFT);    
    else if (ihandler.isKeyTapped(K_RIGHT)) m_pSprite->getDir() == DIR_RIGHT ? m_pSprite->tryMove(DIR_RIGHT) : m_pSprite->tryChangeDirection(DIR_RIGHT);    

    else if (ihandler.isKeyDown(K_DOWN))    m_pSprite->tryMove(DIR_DOWN);    
    else if (ihandler.isKeyDown(K_UP))      m_pSprite->tryMove(DIR_UP);    
    else if (ihandler.isKeyDown(K_LEFT))    m_pSprite->tryMove(DIR_LEFT);    
    else if (ihandler.isKeyDown(K_RIGHT))   m_pSprite->tryMove(DIR_RIGHT);    

    else
    {
        if (!m_pSprite->getNextTile()->isWalkable() && 
            m_pSprite->getState() != Sprite::S_MOVING)
        {            
            if ((m_pSprite->getDir() == DIR_DOWN  && !ihandler.isKeyDown(K_DOWN))   ||
                (m_pSprite->getDir() == DIR_UP    && !ihandler.isKeyDown(DIR_UP))   ||
                (m_pSprite->getDir() == DIR_LEFT  && !ihandler.isKeyDown(DIR_LEFT)) ||
                (m_pSprite->getDir() == DIR_RIGHT && !ihandler.isKeyDown(DIR_RIGHT)))
            {
                m_pSprite->setState(Sprite::S_IDLE);
                m_pSprite->setWalkingAnimation(false);
            }
        }
    }

    m_pSprite->update();
}

void Player::render()
{
    m_pSprite->render();
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