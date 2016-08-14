/* ======================
   Date: 31/7/2016
   Author: Alex Koukoulas
   ====================== */

#pragma once

#include "../../mydef.h"

#include <unordered_map>
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

	const pokemonParty_t& getLocalPokemonParty() const;
	const pokemonParty_t& getEnemyPokemonParty() const;
	
	Pokemon& getActiveLocalPokemon() const;
	Pokemon& getActiveEnemyPokemon() const;

	bool isWildBattle() const;

	std::shared_ptr<TextureResource> getLocalPokemonActorTexture(const std::string& pokemonName) const;
	std::shared_ptr<TextureResource> getEnemyPokemonActorTexture(const std::string& pokemonName) const;

	std::shared_ptr<TextureResource> getLocalPokemonStatsTexture() const;
	std::shared_ptr<TextureResource> getEnemyPokemonStatsTexture() const;

	std::shared_ptr<TextureResource> getNormalTrainerAtlas() const;
	std::shared_ptr<TextureResource> getDarkTrainerAtlas() const;

	uiComponentStack_t& getUIComponentStack();

private:

    const pokemonParty_t& m_localPokemon;
    const pokemonParty_t& m_enemyPokemon;

	std::unordered_map<std::string, std::shared_ptr<TextureResource>> m_localPokemonActorTextures;
	std::unordered_map<std::string, std::shared_ptr<TextureResource>> m_enemyPokemonActorTextures;

    const BattleType      m_battleType;
    
    uiComponentStack_t m_uiComponentStack;

    std::shared_ptr<TextureResource> m_normalTrainerAtlas;
    std::shared_ptr<TextureResource> m_darkTrainerAtlas;
	std::shared_ptr<TextureResource> m_enemyPokemonStatsTexture;
	std::shared_ptr<TextureResource> m_localPokemonStatsTexture;
    std::shared_ptr<TextureResource> m_textboxTexture;
    std::unique_ptr<FSMState> m_activeState;
};