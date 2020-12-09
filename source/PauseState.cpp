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
    TextureManager::Instance()->drawTexture("PauseState", this->x, this->y);

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
    SDL_GetWindowSize(Game::Instance()->getWindow(), &this->width, &this->height);

    // loading of colors, fonts and textures
    TextureManager::Instance()->loadColor("white", 255, 255, 255, 255);

    TextureManager::Instance()->loadFont("ttf/OxygenRegular.ttf", 30, "oxyReg30");

    TextureManager::Instance()->loadImageTexture("assets/strand_1024_768.jpg", "pauseBackground");

    TextureManager::Instance()->loadTextTexture("oxyReg30", "Pausing the Game", "white", "PauseState", TextQuality::BLENDED);
    TextureManager::Instance()->loadTextTexture("oxyReg30", "Resume", "white", "Resume", TextQuality::BLENDED);
    TextureManager::Instance()->loadTextTexture("oxyReg30", "Menu", "white", "Back", TextQuality::BLENDED);

    this->x = (this->width - TextureManager::Instance()->getWidthOfTexture("PauseState")) / 2;
    this->y = 50;

    this->gameObjects.push_back(new MenuButton(this->width / 2 - 75, this->width / 2 + 75,
                                               125, 175, "Resume", 0, 0, 85, 170, 255, resumePlay));

    this->gameObjects.push_back(new MenuButton(this->width / 2 - 75, this->width / 2 + 75,
                                               200, 250, "Back", 0, 0, 85, 170, 255, pauseToMain));

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

    InputManager::Instance()->reset();

    return true;
}
