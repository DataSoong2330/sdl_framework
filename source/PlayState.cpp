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
    Game::Instance()->getGameStateMachine()->pushState(new PauseState());
}

void PlayState::playToGameOver()
{
    Game::Instance()->getGameStateMachine()->changeState(new GameOverState());
}

void PlayState::returnToMenu()
{
    Game::Instance()->getGameStateMachine()->changeState(new MenuState());
}

// update the state
void PlayState::update()
{
    if(this->gameObjects.size())
    {
        for(unsigned i = 0; i < this->gameObjects.size(); i++)
        this->gameObjects[i]->update();
    }
}

// render the state
void PlayState::render()
{
    // draws the textures with their id and a coordinate
    TextureManager::Instance()->drawTexture("playBackground", 0, 0);
    TextureManager::Instance()->drawTexture("Playing", (this->screenSize.w - TextureManager::Instance()->getWidthOfTexture("Playing")) / 2, 50);

    // draws all the game objects
    if(this->gameObjects.size() > 0)
    {
        for(unsigned i = 0; i < this->gameObjects.size(); i++)
            this->gameObjects[i]->draw();
    }
}

// what happens on entering the state
bool PlayState::onEnter()
{
    InputManager::Instance()->reset();

    // gets the current screen size, necessary for alignment of the buttons and textures
    SDL_GetWindowSize(Game::Instance()->getWindow(), &this->screenSize.w, &this->screenSize.h);
    this->screenSize.x = this->screenSize.y = 0;

    // loading of colors, fonts and textures
    TextureManager::Instance()->loadColor("white", 255, 255, 255, 255);

    TextureManager::Instance()->loadFont("ttf/OxygenRegular.ttf", 30, "oxyReg30");

    TextureManager::Instance()->loadImageTexture("assets/milkyway_1024_768.jpg", "playBackground");

    TextureManager::Instance()->loadTextTexture("oxyReg30", "Playing the Game", "white", "Playing", TextQuality::BLENDED);
    TextureManager::Instance()->loadTextTexture("oxyReg30", "Pause", "white", "Pause", TextQuality::BLENDED);
    TextureManager::Instance()->loadTextTexture("oxyReg30", "Dead", "white", "Dead", TextQuality::BLENDED);
    TextureManager::Instance()->loadTextTexture("oxyReg30", "Return", "white", "Return", TextQuality::BLENDED);

    Button values;
    values.x1 = 10, values.x2 = 160;
    values.y1 = 10, values.y2 = 60;
    values.textureID = "Pause", values.colorID = "white", values.fontID = "oxyReg30";
    values.angle = 0.0d;
    values.r = 0, values.g = 85, values.b = 170, values.a = 255;
    values.viewportID = "menu";
    values.func = playToPause;

    this->gameObjects.push_back(new MenuButton(values));

    values.y1 = 70, values.y2 = 120;
    values.textureID = "Return";
    values.func = returnToMenu;

    this->gameObjects.push_back(new MenuButton(values));

    values.y1 = 130, values.y2 = 180;
    values.textureID = "Dead";
    values.func = playToGameOver;

    this->gameObjects.push_back(new MenuButton(values));

    return true;
}

// what happens on exiting the state
bool PlayState::onExit()
{
    TextureManager::Instance()->removeColor("white");

    TextureManager::Instance()->removeFont("oxyReg30");

    TextureManager::Instance()->removeTexture("playBackground");
    TextureManager::Instance()->removeTexture("Playing");

    TextureManager::Instance()->removeTexture("Pause");
    TextureManager::Instance()->removeTexture("Dead");
    TextureManager::Instance()->removeTexture("Return");

    for(auto &item : this->gameObjects)
    {
        delete item;
    }

    this->gameObjects.clear();

    InputManager::Instance()->reset();

    return true;
}
