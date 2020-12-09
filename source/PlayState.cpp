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
    for(unsigned i = 0; i < this->gameObjects.size(); i++)
        this->gameObjects[i]->update();
}

// render the state
void PlayState::render()
{
    // draws the textures with their id and a coordinate
    TextureManager::Instance()->drawTexture("playBackground", 0, 0);
    TextureManager::Instance()->drawTexture("Playing", this->x, this->y);

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
    SDL_GetWindowSize(Game::Instance()->getWindow(), &this->width, &this->height);

    // loading of colors, fonts and textures
    TextureManager::Instance()->loadColor("white", 255, 255, 255, 255);

    TextureManager::Instance()->loadFont("OxygenRegular.ttf", 30, "oxyReg30");

    TextureManager::Instance()->loadImageTexture("milkyway_1024_768.jpg", "playBackground");

    TextureManager::Instance()->loadTextTexture("oxyReg30", "Playing the Game", "white", "Playing", TextQuality::BLENDED);
    TextureManager::Instance()->loadTextTexture("oxyReg30", "Pause", "white", "Pause", TextQuality::BLENDED);
    TextureManager::Instance()->loadTextTexture("oxyReg30", "Dead", "white", "Dead", TextQuality::BLENDED);
    TextureManager::Instance()->loadTextTexture("oxyReg30", "Return", "white", "Return", TextQuality::BLENDED);

    // texture alignment
    this->x = (this->width - TextureManager::Instance()->getWidthOfTexture("Playing")) / 2;
    this->y = 50;

    this->gameObjects.push_back(new MenuButton(25, 175, 25, 75, "Pause", 0.0d, 0, 85, 170, 255, playToPause));

    this->gameObjects.push_back(new MenuButton(25, 175, 100, 150, "Return", 0.0d, 0, 85, 170, 255, returnToMenu));

    this->gameObjects.push_back(new MenuButton(25, 175, 175, 225, "Dead", 0.0d, 0, 85, 170, 255, playToGameOver));

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

    for(unsigned i = 0; i < this->gameObjects.size(); i++)
        delete this->gameObjects[i];

    this->gameObjects.clear();

    InputManager::Instance()->reset();

    return true;
}
