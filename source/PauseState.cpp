#include "../header/Game.hpp"
#include "../header/InputManager.hpp"
#include "../header/MenuState.hpp"
#include "../header/PauseState.hpp"
#include "../header/TextureManager.hpp"

const std::string PauseState::pauseID = "PAUSE";

// leaves the state to the main menu
void PauseState::pauseToMain()
{
    Game::Instance()->getGameStateMachine()->changeState(new MenuState());
}

// leaves the state and goes to the previous state
void PauseState::resumePlay()
{
    Game::Instance()->getGameStateMachine()->popState();
}

// update the state, input for example
void PauseState::update()
{
    for(unsigned i = 0; i < this->gameObjects.size(); i++)
        this->gameObjects[i]->update();
}

// renders the state
void PauseState::render()
{
    // draws the textures with their id and a coordinate
    TextureManager::Instance()->drawTexture("pauseBackground", 0, 0);
    TextureManager::Instance()->drawTexture("PauseState", (this->screenSize.w - TextureManager::Instance()->getWidthOfTexture("PauseState")) / 2, 50);

    // draws all the game objects
    if(this->gameObjects.size() > 0)
    {
        for(unsigned i = 0; i < this->gameObjects.size(); i++)
            this->gameObjects[i]->draw();
    }
}

// loads all necessary resources for the state
bool PauseState::onEnter()
{
    InputManager::Instance()->reset();

    // gets the current screen size, necessary for alignment of the buttons and textures
    SDL_GetWindowSize(Game::Instance()->getWindow(), &this->screenSize.w, &this->screenSize.h);
    this->screenSize.x = this->screenSize.y = 0;

    // loading of colors, fonts and textures
    TextureManager::Instance()->loadColor("white", 255, 255, 255, 255);

    TextureManager::Instance()->loadFont("ttf/OxygenRegular.ttf", 30, "oxyReg30");

    TextureManager::Instance()->loadImageTexture("assets/strand_1024_768.jpg", "pauseBackground");

    TextureManager::Instance()->loadTextTexture("oxyReg30", "Pausing the Game", "white", "PauseState", TextQuality::BLENDED);
    TextureManager::Instance()->loadTextTexture("oxyReg30", "Resume", "white", "Resume", TextQuality::BLENDED);
    TextureManager::Instance()->loadTextTexture("oxyReg30", "Menu", "white", "Back", TextQuality::BLENDED);

    Button values;
    values.x1 = 10, values.x2 = 160;
    values.y1 = 10, values.y2 = 60;
    values.textureID = "Resume", values.colorID = "white", values.fontID = "oxyReg30";
    values.angle = 0.0d;
    values.r = 0, values.g = 85, values.b = 170, values.a = 255;
    values.viewportID = "menu";
    values.func = resumePlay;

    this->gameObjects.push_back(new MenuButton(values));

    values.y1 = 70, values.y2 = 120;
    values.textureID = "Back";
    values.func = pauseToMain;

    this->gameObjects.push_back(new MenuButton(values));

    return true;
}

// removes all resources loaded on enter
bool PauseState::onExit()
{
    TextureManager::Instance()->removeColor("white");

    TextureManager::Instance()->removeFont("oxyReg30");

    TextureManager::Instance()->removeTexture("pauseBackground");
    TextureManager::Instance()->removeTexture("PauseState");
    TextureManager::Instance()->removeTexture("Resume");
    TextureManager::Instance()->removeTexture("Back");

    for(auto &item : this->gameObjects)
    {
        delete item;
    }

    this->gameObjects.clear();

    InputManager::Instance()->reset();

    return true;
}
