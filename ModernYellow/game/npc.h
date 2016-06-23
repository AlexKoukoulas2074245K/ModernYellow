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

    Npc(const int32 texU,
        const int32 texV,
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

    void darken();

private:

    static const uint32 NPC_FLAG_DYNAMIC = 1 << 0;
    static const uint32 NPC_FLAG_TRAINER = 1 << 1;

    static const uint32 NPC_STATIC_RESET_DELAY  = 120;
    static const uint32 NPC_MOVE_DECISION_DELAY = 200;
    static const uint32 NPC_MOVE_DECISION_VAR   = 80;

private:
    
    uint32 m_flags;
    mutable uint32 m_decisionTimer;
    mutable uint32 m_staticRestTimer;
    string m_dialogue;
    std::weak_ptr<const Level> m_pLevelRef;
    Direction m_initDir;
    
};