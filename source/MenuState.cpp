#include "../header/Game.hpp"
#include "../header/InputManager.hpp"
#include "../header/MenuState.hpp"
#include "../header/PlayState.hpp"
#include "../header/SoundManager.hpp"
#include "../header/TextureManager.hpp"

MenuState::MenuState(GameStateMachine& stateMachine) : GameState(stateMachine)
{
}

// update the state
bool MenuState::update()
{
    return GameState::update();
}

// render the state
void MenuState::render()
{
    GameState::render();
}

bool MenuState::handleEvents()
{
    return GameState::handleEvents();
}

// what happens on entering the state
bool MenuState::onEnter(std::string fileName)
{
    this->functionMap["menuToPlay"] = [this] ()
    {
        this->requestStackPop();
        this->requestStackPush(States::Play);
    };
    this->functionMap["menuToSetting"] = [this] ()
    {
        this->requestStackPop();
        this->requestStackPush(States::Setting);
    };
    this->functionMap["exitFromMenu"] = [this] ()
    {
        Game::Instance()->quit();
    };

    return GameState::onEnter(fileName);
}

// what happens on exiting the state
bool MenuState::onExit()
{
    return GameState::onExit();
}
