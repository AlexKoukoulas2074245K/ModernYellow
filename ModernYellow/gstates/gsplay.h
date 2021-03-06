/* ======================
   Date: 30/5/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once

#include "gstate.h"
#include "../mydef.h"

#include <vector>

class UIBox;
class Level;
class TextureResource;
class BattleController;
class Player;
class GSPlay final: public GState
{
public:
    
    GSPlay();

    ~GSPlay();

    void update() override;

    void render() override;

private:

    std::shared_ptr<Level> m_pLevel;
    std::unique_ptr<Player> m_pPlayer;       
    std::unique_ptr<BattleController> m_pBattleController;
    bool m_uiActions;
    bool m_encounterInProgress;
};