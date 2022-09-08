#include "../header/Game.hpp"
#include "../header/InputManager.hpp"
#include "../header/MenuState.hpp"
#include "../header/PauseState.hpp"
#include "../header/SoundManager.hpp"
#include "../header/TextureManager.hpp"

PauseState::PauseState(GameStateMachine& stateMachine) : GameState(stateMachine)
{
}

// update the state, input for example
bool PauseState::update()
{
    return GameState::update();
}

// renders the state
void PauseState::render()
{
    GameState::render();
}

bool PauseState::handleEvents()
{
    return GameState::handleEvents();
}

// loads all necessary resources for the state
bool PauseState::onEnter(std::string fileName)
{
    this->functionMap["pauseToMain"] = [this] ()
    {
        this->requestStackClear();
        this->requestStackPush(States::Menu);
    };
    this->functionMap["resumePlay"] = [this] ()
    {
        this->requestStackPop();
        this->requestStackPush(States::Play);
    };

    return GameState::onEnter(fileName);
}

// removes all resources loaded on enter
bool PauseState::onExit()
{
    return GameState::onExit();
}
