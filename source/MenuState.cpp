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

    TextureManager::Instance()->drawTexture("background", 0, 0, nullptr, "main");
    TextureManager::Instance()->drawTexture("Titel", (this->screenSize.w - TextureManager::Instance()->getWidthOfTexture("Titel")) / 2, 50, NULL, "main");

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
    SDL_GetWindowSize(Game::Instance()->getWindow(), &this->screenSize.w, &this->screenSize.h);
    this->screenSize.x = this->screenSize.y = 0;

    // loading a sound and starts playing it
    SoundManager::Instance()->load("music/Bach_Air_On_the_G_String.mp3", "music", SoundType::SOUND_MUSIC);
    SoundManager::Instance()->playMusic("music", 0);

    TextureManager::Instance()->addViewport(170, 0, this->screenSize.w, this->screenSize.h, "main");
    TextureManager::Instance()->addViewport(0, 0, 170, this->screenSize.h, "menu");

    // loading of colors, fonts and textures
    TextureManager::Instance()->loadColor("white", 255, 255, 255, 255);

    TextureManager::Instance()->loadFont("ttf/OxygenRegular.ttf", 30, "oxyReg30");

    TextureManager::Instance()->loadImageTexture("assets/space_galaxy_shine_1024x768.jpg", "background");
    TextureManager::Instance()->loadImageTexture("nullptr", "none");

    TextureManager::Instance()->loadTextTexture("oxyReg30", "SDL Framework", "white", "Titel", TextQuality::BLENDED);
    TextureManager::Instance()->loadTextTexture("oxyReg30", "Play", "white", "Play", TextQuality::BLENDED);
    TextureManager::Instance()->loadTextTexture("oxyReg30", "Exit", "white", "Exit", TextQuality::BLENDED);

    Button values;
    values.x1 = 10, values.x2 = 160;
    values.y1 = 10, values.y2 = 60;
    values.textureID = "Play", values.colorID = "white", values.fontID = "oxyReg30";
    values.angle = 0.0d;
    values.r = 0, values.g = 85, values.b = 170, values.a = 255;
    values.func = menuToPlay;
    values.viewportID = "menu";
    this->gameObjects.push_back(new MenuButton(values));

    values.y1 = 70, values.y2 = 120;
    values.textureID = "Exit";
    values.func = exitFromMenu;
    this->gameObjects.push_back(new MenuButton(values));

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

    TextureManager::Instance()->removeViewport("leftViewport");
    TextureManager::Instance()->removeViewport("rightViewport");

    for(auto &item : this->gameObjects)
    {
        delete item;
    }

    this->gameObjects.clear();

    InputManager::Instance()->reset();

    return true;
}
