#include "../header/Game.hpp"
#include "../header/InputManager.hpp"
#include "../header/MenuState.hpp"
#include "../header/PlayState.hpp"
#include "../header/SoundManager.hpp"
#include "../header/TextureManager.hpp"

// state ID
const std::string MenuState::menuID = "MENU";

// switch state to play state
void MenuState::menuToPlay()
{
    Game::Instance()->getGameStateMachine()->changeState(new PlayState(), "states/playState.json");
}

// quit the game
void MenuState::exitFromMenu()
{
    Game::Instance()->quit();
}

// update the state
void MenuState::update()
{
    GameState::update();
}

// render the state
void MenuState::render()
{
    GameState::render();
}

// what happens on entering the state
bool MenuState::onEnter(std::string fileName)
{
    this->functionMap["menuToPlay"] = menuToPlay;
    this->functionMap["exitFromMenu"] = exitFromMenu;

    return GameState::onEnter(fileName);
}

// what happens on exiting the state
bool MenuState::onExit()
{
    return GameState::onExit();
}
