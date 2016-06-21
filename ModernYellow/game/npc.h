/* ======================
   Date: 19/6/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once

#include "sprite.h"

#include <string>

using std::string;

class Npc: public Sprite
{
public:

    Npc(const uint32 texU,
        const uint32 texV,
        const std::shared_ptr<Tile> pInitTile,
        const Direction initDir,
        const std::shared_ptr<const Level> pLevelRef,
        const std::shared_ptr<TextureResource>& pAtlas,
        const bool movingNpc,
        const bool isTrainer,
        const string& dialogue);

    ~Npc();

    const string& getDialogue() const;

    int isDynamic() const;

    int isTrainer() const;

    void update() override; 

private:

    static const uint32 NPC_FLAG_DYNAMIC = 1 << 0;
    static const uint32 NPC_FLAG_TRAINER = 1 << 1;

    static const uint32 NPC_MOVE_DECISION_DELAY = 200;

private:
    
    uint32 m_flags;
    uint32 m_decisionTimer;
    string m_dialogue;
    std::weak_ptr<const Level> m_pLevelRef;
    
};