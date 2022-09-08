#include "../header/Game.hpp"
#include "../header/GameOverState.hpp"
#include "../header/InputManager.hpp"
#include "../header/MenuState.hpp"
#include "../header/PauseState.hpp"
#include "../header/PlayState.hpp"
#include "../header/TextureManager.hpp"

PlayState::PlayState(GameStateMachine& stateMachine) : GameState(stateMachine)
{
}

// update the state
bool PlayState::update()
{
    return GameState::update();
}

// render the state
void PlayState::render()
{
    GameState::render();
}

bool PlayState::handleEvents()
{
    return GameState::handleEvents();
}

// what happens on entering the state
bool PlayState::onEnter(std::string fileName)
{
    this->functionMap["playToPause"] = [this] ()
    {
        this->requestStackPop();
        this->requestStackPush(States::Pause);
    };
    this->functionMap["playToGameOver"] = [this] ()
    {
        this->requestStackPop();
        this->requestStackPush(States::GameOver);
    };
    this->functionMap["returnToMenu"] = [this] ()
    {
        this->requestStackPop();
        this->requestStackPush(States::Menu);
    };

    return GameState::onEnter(fileName);
}

// what happens on exiting the state
bool PlayState::onExit()
{
    return GameState::onExit();
}
