/* ======================
   Date: 30/5/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once

#include "gstate.h"
#include <memory>
#include <vector>

class Level;
class TextureResource;
class Player;
class Sprite;
class GSPlay final: public GState
{
public:
    using npcs_t = std::vector<std::unique_ptr<Sprite>>;

    GSPlay();

    ~GSPlay();

    void update() override;

    void render() override;

private:

    std::shared_ptr<Level> m_pLevel;
    std::unique_ptr<Player> m_pPlayer;
    npcs_t m_npcs;
};