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
    const string& dialogue,
    const string& dialogueAfterInteraction):

    Sprite(texU, texV, pInitTile, initDir, pLevelRef, pAtlas), 
    m_initDir(initDir),
    m_pLevelRef(pLevelRef),
    m_decisionTimer(0U),
    m_dialogue(std::move(dialogue)),
    m_dialogueAfterInteraction(std::move(dialogueAfterInteraction)),
    m_staticRestTimer(0U),
    m_flags(0U)
{
    if (movingNpc) m_flags |= NPC_FLAG_DYNAMIC;
    if (isTrainer) m_flags |= NPC_FLAG_TRAINER;    
    
    srand((uint32) time(NULL));

    m_decisionTimer = NPC_MOVE_DECISION_DELAY + (rand() % NPC_MOVE_DECISION_VAR);
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
            m_decisionTimer = NPC_MOVE_DECISION_DELAY + (rand() % NPC_MOVE_DECISION_VAR);

            Direction randDir = (Direction)(rand() % 4);

            std::shared_ptr<Tile> nextTile;

            switch (randDir)
            {
                case DIR_DOWN:  nextTile = pLevelRef->getTileBelow(getCurrTile()); break;
                case DIR_UP:    nextTile = pLevelRef->getTileAbove(getCurrTile()); break;
                case DIR_LEFT:  nextTile = pLevelRef->getTileLeftOf(getCurrTile()); break;
                case DIR_RIGHT: nextTile = pLevelRef->getTileRightOf(getCurrTile()); break;
            }

            if (!nextTile->isWalkable() || nextTile->getTileType() == Tile::TT_LEDGE)
            {
                tryChangeDirection(randDir);
            }
            else
            {
                tryMove(randDir);
            }
        }
    }
    else if (!isDynamic())
    {
        if (--m_staticRestTimer <= 0)
        {
            m_staticRestTimer = NPC_STATIC_RESET_DELAY;
            tryChangeDirection(m_initDir);
        }
    }

    Sprite::update();
}

void Npc::switchPaletteTo(const uint32 color)
{
    Sprite::switchPaletteTo(color);
}

void Npc::darken()
{
    Sprite::darken();
}

void Npc::switchDialogue()
{
    m_dialogue = m_dialogueAfterInteraction;
}

const string& Npc::getDialogue() const
{
    m_staticRestTimer = NPC_STATIC_RESET_DELAY;
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
