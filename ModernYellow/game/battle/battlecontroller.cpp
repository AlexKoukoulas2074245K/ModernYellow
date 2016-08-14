/* ======================
   Date: 31/7/2016
   Author: Alex Koukoulas
   ====================== */

#include "battlecontroller.h"
#include "battlefsm/fsmstatedarkintro.h"
#include "../uicomps/uicomponent.h"
#include "../../resources/sresmanager.h"
#include "../../resources/textureresource.h"
#include "../pokemon.h"

extern pRenderer_t g_pRenderer;
extern uint32 g_height;
extern uint32 g_scale;

/* ==============
   Public Methods
   ============== */
BattleController::BattleController(
    const pokemonParty_t& localPokemon,
    const pokemonParty_t& enemyPokemon,
    const BattleType& battleType):
   
    m_localPokemon(localPokemon),
    m_enemyPokemon(enemyPokemon),
    m_battleType(battleType),
    m_textboxTexture(castResToTex(resmanager.loadResource("misctex/textbox.png", RT_TEXTURE))),
    m_normalTrainerAtlas(castResToTex(resmanager.loadResource("tilemaps/traineratlas.png", RT_TEXTURE))),
    m_darkTrainerAtlas(castResToTex(resmanager.loadResource("tilemaps/dtraineratlas.png", RT_TEXTURE))),
	m_enemyPokemonStatsTexture(castResToTex(resmanager.loadResource("misctex/enemy_stats.png", RT_TEXTURE))),
	m_localPokemonStatsTexture(castResToTex(resmanager.loadResource("misctex/player_stats.png", RT_TEXTURE))),
    m_activeState(std::make_unique<FSMStateDarkIntro>(*this))
{
	for (const auto& pokemon: m_localPokemon)
		m_localPokemonActorTextures[pokemon->getName()] = castResToTex(resmanager.loadResource("pkmnback/" + pokemon->getName() + ".png", RT_TEXTURE));

	for (const auto& pokemon: m_enemyPokemon)
		m_enemyPokemonActorTextures[pokemon->getName()] = castResToTex(resmanager.loadResource("pkmnfront/" + pokemon->getName() + ".png", RT_TEXTURE));
}

BattleController::~BattleController(){}

void BattleController::update()
{
    m_activeState->update();
    if (m_activeState->isFinished())
    {
        m_activeState = m_activeState->getSuccessor();
    }
}

void BattleController::render()
{
    SDL_SetRenderDrawColor(g_pRenderer.get(),
        RED(envcolors::EC_WHITE),
        GREEN(envcolors::EC_WHITE),
        BLUE(envcolors::EC_WHITE),
        ALPHA(envcolors::EC_WHITE));
    SDL_RenderClear(g_pRenderer.get());
        
    SDLRender(
        g_pRenderer, 
        m_textboxTexture->getTexture().get(),
        0,
        g_height - m_textboxTexture->getScaledHeight(),
        m_textboxTexture->getScaledWidth(),
        m_textboxTexture->getScaledHeight());
    
    m_activeState->render();

}

const BattleController::pokemonParty_t& BattleController::getLocalPokemonParty() const
{
	return m_localPokemon;
}

const BattleController::pokemonParty_t& BattleController::getEnemyPokemonParty() const
{
	return m_enemyPokemon;
}

Pokemon& BattleController::getActiveLocalPokemon() const
{
	return *m_localPokemon[0];
}

Pokemon& BattleController::getActiveEnemyPokemon() const
{
	return *m_enemyPokemon[0];
}

bool BattleController::isWildBattle() const
{
	return m_battleType == BattleType::BT_ENCOUNTER;
}

std::shared_ptr<TextureResource> BattleController::getLocalPokemonActorTexture(const std::string& pokemonName) const
{
	return m_localPokemonActorTextures.at(pokemonName);
}

std::shared_ptr<TextureResource> BattleController::getEnemyPokemonActorTexture(const std::string& pokemonName) const
{
	return m_enemyPokemonActorTextures.at(pokemonName);
}

std::shared_ptr<TextureResource> BattleController::getLocalPokemonStatsTexture() const
{
	return m_localPokemonStatsTexture;
}

std::shared_ptr<TextureResource> BattleController::getEnemyPokemonStatsTexture() const
{
	return m_enemyPokemonStatsTexture;
}

std::shared_ptr<TextureResource> BattleController::getNormalTrainerAtlas() const
{
	return m_normalTrainerAtlas;
}

std::shared_ptr<TextureResource> BattleController::getDarkTrainerAtlas() const
{
	return m_darkTrainerAtlas;
}

BattleController::uiComponentStack_t& BattleController::getUIComponentStack()
{
	return m_uiComponentStack;
}