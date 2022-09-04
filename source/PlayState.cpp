#include "../header/Game.hpp"
#include "../header/GameOverState.hpp"
#include "../header/InputManager.hpp"
#include "../header/MenuState.hpp"
#include "../header/PauseState.hpp"
#include "../header/PlayState.hpp"
#include "../header/TextureManager.hpp"

// state ID
const std::string PlayState::playID = "PLAY";

void PlayState::playToPause()
{
    Game::Instance()->getGameStateMachine()->pushState(new PauseState(), "states/pauseState.json");
}

void PlayState::playToGameOver()
{
    Game::Instance()->getGameStateMachine()->changeState(new GameOverState(), "states/gameOverState.json");
}

void PlayState::returnToMenu()
{
    Game::Instance()->getGameStateMachine()->changeState(new MenuState(), "states/menuState.json");
}

// update the state
void PlayState::update()
{
    GameState::update();
}

// render the state
void PlayState::render()
{
    GameState::render();
}

// what happens on entering the state
bool PlayState::onEnter(std::string fileName)
{
    this->functionMap["playToPause"] = playToPause;
    this->functionMap["playToGameOver"] = playToGameOver;
    this->functionMap["returnToMenu"] = returnToMenu;

    return GameState::onEnter(fileName);
}

// what happens on exiting the state
bool PlayState::onExit()
{
    return GameState::onExit();
}
