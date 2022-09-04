#include "../header/Game.hpp"
#include "../header/InputManager.hpp"
#include "../header/MenuState.hpp"
#include "../header/PauseState.hpp"
#include "../header/SoundManager.hpp"
#include "../header/TextureManager.hpp"

const std::string PauseState::pauseID = "PAUSE";

// leaves the state to the main menu
void PauseState::pauseToMain()
{
    Game::Instance()->getGameStateMachine()->changeState(new MenuState(), "states/menuState.json");
}

// leaves the state and goes to the previous state
void PauseState::resumePlay()
{
    Game::Instance()->getGameStateMachine()->popState();
}

// update the state, input for example
void PauseState::update()
{
    GameState::update();
}

// renders the state
void PauseState::render()
{
    GameState::render();
}

// loads all necessary resources for the state
bool PauseState::onEnter(std::string fileName)
{
    this->functionMap["pauseToMain"] = pauseToMain;
    this->functionMap["resumePlay"] = resumePlay;

    return GameState::onEnter(fileName);
}

// removes all resources loaded on enter
bool PauseState::onExit()
{
    return GameState::onExit();
}
