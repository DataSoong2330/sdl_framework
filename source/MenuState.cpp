#include "../header/Game.hpp"
#include "../header/InputManager.hpp"
#include "../header/MenuState.hpp"
#include "../header/PlayState.hpp"
#include "../header/SoundManager.hpp"
#include "../header/TextureManager.hpp"

// state ID
const std::string MenuState::menuID = "MENU";

// switch state to play state
void MenuState::menuToPlay()
{
    Game::Instance()->getGameStateMachine()->changeState(new PlayState());
}

// quit the game
void MenuState::exitFromMenu()
{
    Game::Instance()->quit();
}

// update the state
void MenuState::update()
{
    if(InputManager::Instance()->isKeyTriggered(SDL_SCANCODE_SPACE))
    {
        if(SoundManager::Instance()->isMusicPlaying() &&
           !SoundManager::Instance()->isMusicPaused())
        {
            std::cout << "Abgespielte Musik wird pausiert" << "\n";
            SoundManager::Instance()->pauseMusic();
        }
        else if(SoundManager::Instance()->isMusicPaused())
        {
            std::cout << "Pausierte Musik wird wieder abgespielt" << "\n";
            SoundManager::Instance()->resumeMusic();
        }
        else if(!SoundManager::Instance()->isMusicPaused() &&
                !SoundManager::Instance()->isMusicPlaying())
        {
            std::cout << "Musik wird abgespielt" << "\n";
            SoundManager::Instance()->playMusic("music", 0);
        }
    }
    if(InputManager::Instance()->isKeyTriggered(SDL_SCANCODE_ESCAPE))
    {
        if(SoundManager::Instance()->isMusicPlaying() ||
           SoundManager::Instance()->isMusicPaused())
            SoundManager::Instance()->haltMusic();
    }

    if(this->gameObjects.size() > 0)
    {
        for(unsigned i = 0; i < this->gameObjects.size(); i++)
            this->gameObjects[i]->update();
    }
}

// render the state
void MenuState::render()
{
    // draws the textures with their id and a coordinate
    TextureManager::Instance()->drawTexture("background", 0, 0);
    TextureManager::Instance()->drawTexture("Titel", this->x, this->y);

    // draws all the game objects
    if(this->gameObjects.size() > 0)
    {
        for(unsigned i = 0; i < this->gameObjects.size(); i++)
        this->gameObjects[i]->draw();
    }
}

// what happens on entering the state
bool MenuState::onEnter()
{
    InputManager::Instance()->reset();

    // gets the current screen size, necessary for alignment of the buttons and textures
    SDL_GetWindowSize(Game::Instance()->getWindow(), &this->width, &this->height);

    // loading a sound and starts playing it
    SoundManager::Instance()->load("Bach_Air_On_the_G_String.mp3", "music", SoundType::SOUND_MUSIC);
    SoundManager::Instance()->playMusic("music", 0);

    // loading of colors, fonts and textures
    TextureManager::Instance()->loadColor("white", 255, 255, 255, 255);

    TextureManager::Instance()->loadFont("OxygenRegular.ttf", 30, "oxyReg30");

    TextureManager::Instance()->loadImageTexture("space_galaxy_shine_1024x768.jpg", "background");

    TextureManager::Instance()->loadTextTexture("oxyReg30", "SDL Framework (in development)", "white", "Titel", TextQuality::BLENDED);
    TextureManager::Instance()->loadTextTexture("oxyReg30", "Play", "white", "Play", TextQuality::BLENDED);
    TextureManager::Instance()->loadTextTexture("oxyReg30", "Exit", "white", "Exit", TextQuality::BLENDED);

    // texture alignment
    this->x = (this->width - TextureManager::Instance()->getWidthOfTexture("Titel")) / 2;
    this->y = 50;

    this->gameObjects.push_back(new MenuButton(this->width / 2 - 75, this->width / 2 + 75,
                                               125, 175, "Play", 0.0, 0, 85, 170, 255, menuToPlay));

    this->gameObjects.push_back(new MenuButton(this->width / 2 - 75, this->width / 2 + 75,
                                               200, 250, "Exit", 0.0, 0, 85, 170, 255, exitFromMenu));


    return true;
}

// what happens on exiting the state
bool MenuState::onExit()
{
    SoundManager::Instance()->haltMusic();
    SoundManager::Instance()->removeMusic("music");

    TextureManager::Instance()->removeColor("white");

    TextureManager::Instance()->removeFont("oxyReg30");

    TextureManager::Instance()->removeTexture("background");
    TextureManager::Instance()->removeTexture("Titel");
    TextureManager::Instance()->removeTexture("Play");
    TextureManager::Instance()->removeTexture("Exit");

    for(unsigned i = 0; i < this->gameObjects.size(); i++)
        delete this->gameObjects[i];

    this->gameObjects.clear();

    InputManager::Instance()->reset();

    return true;
}
