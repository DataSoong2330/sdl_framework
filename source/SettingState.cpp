#include "../header/SettingState.hpp"

SettingState::SettingState(GameStateMachine& stateMachine) : GameState(stateMachine)
{
}

// update the state
bool SettingState::update()
{
    return GameState::update();
}

void SettingState::render()
{
    GameState::render();
}

bool SettingState::handleEvents()
{
    return GameState::handleEvents();
}

bool SettingState::onEnter(std::string fileName)
{
    this->functionMap["exitToMenu"] = [this] ()
    {
        this->requestStackPop();
        this->requestStackPush(States::Menu);
    };

    return GameState::onEnter(fileName);
}

bool SettingState::onExit()
{
    return GameState::onExit();
}
