/* ======================
   Date: 19/6/2016
   Author: Alex Koukoulas
   ====================== */

#include "npc.h"
#include "level.h"
#include "tile.h"
#include <time.h>

/* ==============
   Public Methods
   ============== */
Npc::Npc(
    const int32 texU,
    const int32 texV,
    const std::shared_ptr<Tile> pInitTile,
    const Direction initDir,
    const std::shared_ptr<const Level> pLevelRef,
    const std::shared_ptr<TextureResource>& pAtlas,
    const bool movingNpc,
    const bool isTrainer,
    const string& dialogue):

    Sprite(texU, texV, pInitTile, initDir, pLevelRef, pAtlas), 
    m_pLevelRef(pLevelRef),
    m_decisionTimer(NPC_MOVE_DECISION_DELAY),
    m_dialogue(dialogue)
{
    if (movingNpc) m_flags |= NPC_FLAG_DYNAMIC;
    if (isTrainer) m_flags |= NPC_FLAG_TRAINER;    

    srand((uint32) time(NULL));
}

Npc::~Npc(){}

void Npc::update()
{
    // Check for frozen npc (player ui actions)
    if (isFrozen()) return;

    // Check for valid level ref
    if (m_pLevelRef.expired()) return;
    auto pLevelRef = m_pLevelRef.lock();

    if (isDynamic() && getState() != S_MOVING)
    {
        m_decisionTimer--;
        if (m_decisionTimer <= 0)
        {
            m_decisionTimer = NPC_MOVE_DECISION_DELAY;

            Direction randDir = (Direction)(rand() % 4);

            std::shared_ptr<Tile> nextTile;

            switch (randDir)
            {
                case DIR_DOWN:  nextTile = pLevelRef->getTileBelow(getCurrTile()); break;
                case DIR_UP:    nextTile = pLevelRef->getTileAbove(getCurrTile()); break;
                case DIR_LEFT:  nextTile = pLevelRef->getTileLeftOf(getCurrTile()); break;
                case DIR_RIGHT: nextTile = pLevelRef->getTileRightOf(getCurrTile()); break;
            }

            if (!nextTile->isWalkable())
            {
                tryChangeDirection(randDir);
            }
            else
            {
                tryMove(randDir);
            }
        }
    }
    Sprite::update();
}

const string& Npc::getDialogue() const
{
    return m_dialogue;
}

int Npc::isDynamic() const
{
    return m_flags & NPC_FLAG_DYNAMIC;
}

int Npc::isTrainer() const
{
    return m_flags & NPC_FLAG_TRAINER;
}
