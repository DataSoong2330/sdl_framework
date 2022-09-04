#include "../header/Game.hpp"
#include "../header/GameOverState.hpp"
#include "../header/InputManager.hpp"
#include "../header/Logfile.hpp"
#include "../header/MenuState.hpp"
#include "../header/PlayState.hpp"
#include "../header/SoundManager.hpp"
#include "../header/TextureManager.hpp"

const std::string GameOverState::gameOverID = "GAMEOVER";

void GameOverState::gameOverToMain()
{
    Game::Instance()->getGameStateMachine()->changeState(new MenuState(), "states/menuState.json");
}

void GameOverState::restartPlay()
{
    Game::Instance()->getGameStateMachine()->changeState(new PlayState(), "states/playState.json");
}

// update the state
void GameOverState::update()
{
    GameState::update();
}

void GameOverState::render()
{
    GameState::render();
}

bool GameOverState::onEnter(std::string fileName)
{
    this->functionMap["gameOverToMain"] = gameOverToMain;
    this->functionMap["restartPlay"] = restartPlay;

    return GameState::onEnter(fileName);
}

bool GameOverState::onExit()
{
    return GameState::onExit();
}
