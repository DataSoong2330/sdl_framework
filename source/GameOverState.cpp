#include "../header/Game.hpp"
#include "../header/GameOverState.hpp"
#include "../header/InputManager.hpp"
#include "../header/Logfile.hpp"
#include "../header/MenuState.hpp"
#include "../header/PlayState.hpp"
#include "../header/SoundManager.hpp"
#include "../header/TextureManager.hpp"

GameOverState::GameOverState(GameStateMachine& stateMachine) : GameState(stateMachine)
{
}

// update the state
bool GameOverState::update()
{
    return GameState::update();
}

void GameOverState::render()
{
    GameState::render();
}

bool GameOverState::handleEvents()
{
    return GameState::handleEvents();
}

bool GameOverState::onEnter(std::string fileName)
{
    this->functionMap["gameOverToMain"] = [this] ()
    {
        this->requestStackPop();
        this->requestStackPush(States::Menu);
    };
    this->functionMap["restartPlay"] = [this] ()
    {
        this->requestStackPop();
        this->requestStackPush(States::Play);
    };

    return GameState::onEnter(fileName);
}

bool GameOverState::onExit()
{
    return GameState::onExit();
}
