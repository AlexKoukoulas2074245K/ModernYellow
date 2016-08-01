/* ======================
   Date: 31/7/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once

#include "../../mydef.h"

#include <vector>
#include <stack>

class Pokemon;
class FSMState;
class TextureResource;
class UIComponent;
class BattleController final
{
public:

    using pokemonParty_t = std::vector<std::unique_ptr<Pokemon>>;
    using uiComponentStack_t = std::stack<std::unique_ptr<UIComponent>>;

    BattleController(
        const pokemonParty_t& localPokemon, 
        const pokemonParty_t& enemyPokemon,
        const BattleType& battleType);

    ~BattleController();
    
    void update();

    void render();

private:

    const pokemonParty_t& m_localPokemon;
    const pokemonParty_t& m_enemyPokemon;
    const BattleType      m_battleType;
    
    uiComponentStack_t m_uiComponentStack;

    std::shared_ptr<TextureResource> m_normalTrainerAtlas;
    std::shared_ptr<TextureResource> m_darkTrainerAtlas;
    std::shared_ptr<TextureResource> m_textboxTexture;
    std::unique_ptr<FSMState> m_activeState;
};