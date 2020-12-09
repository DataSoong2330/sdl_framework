#include "../header/Game.hpp"
#include "../header/GameOverState.hpp"
#include "../header/InputManager.hpp"
#include "../header/MenuState.hpp"
#include "../header/PlayState.hpp"
#include "../header/SoundManager.hpp"
#include "../header/TextureManager.hpp"

const std::string GameOverState::gameOverID = "GAMEOVER";

void GameOverState::gameOverToMain()
{
    Game::Instance()->getGameStateMachine()->changeState(new MenuState());
}

void GameOverState::restartPlay()
{
    Game::Instance()->getGameStateMachine()->changeState(new PlayState());
}

// update the state
void GameOverState::update()
{
    if(InputManager::Instance()->isKeyPressed(SDL_SCANCODE_SPACE))
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
    if(InputManager::Instance()->isKeyPressed(SDL_SCANCODE_ESCAPE))
    {
        if(SoundManager::Instance()->isMusicPlaying() ||
           SoundManager::Instance()->isMusicPaused())
            SoundManager::Instance()->haltMusic();
    }

    for(unsigned i = 0; i < this->gameObjects.size(); i++)
        this->gameObjects[i]->update();
}

void GameOverState::render()
{
    // draws the textures with their id and a coordinate
    TextureManager::Instance()->drawTexture("gameOverBackground", 0, 0);
    TextureManager::Instance()->drawTexture("Over", this->x, this->y);

    // draws all the game objects
    if(this->gameObjects.size() > 0)
    {
        for(unsigned i = 0; i < this->gameObjects.size(); i++)
            this->gameObjects[i]->draw();
    }
}

bool GameOverState::onEnter()
{
    InputManager::Instance()->reset();

    SoundManager::Instance()->load("music/Beethoven_Moonlight_Sonata_Instrumental.mp3", "music", SoundType::SOUND_MUSIC);
    SoundManager::Instance()->playMusic("music", 0);

    // gets the current screen size, necessary for alignment of the buttons and textures
    SDL_GetWindowSize(Game::Instance()->getWindow(), &this->width, &this->height);

    // loading of colors, fonts and textures
    TextureManager::Instance()->loadColor("white", 255, 255, 255, 255);

    TextureManager::Instance()->loadFont("ttf/OxygenRegular.ttf", 30, "oxyReg30");

    TextureManager::Instance()->loadImageTexture("assets/crystal_heads_1024_768.jpg", "gameOverBackground");

    TextureManager::Instance()->loadTextTexture("oxyReg30", "He's dead, Jim!", "white", "Over", TextQuality::BLENDED);
    TextureManager::Instance()->loadTextTexture("oxyReg30", "Neu", "white", "Neu", TextQuality::BLENDED);
    TextureManager::Instance()->loadTextTexture("oxyReg30", "Menu", "white", "Back", TextQuality::BLENDED);

    this->x = (this->width - TextureManager::Instance()->getWidthOfTexture("Over")) / 2;
    this->y = 50;

    this->gameObjects.push_back(new MenuButton(this->width - 25 - 150, this->width - 25,
                                               125, 175, "Neu", 0, 0, 85, 170, 255, restartPlay));
    this->gameObjects.push_back(new MenuButton(this->width - 25 - 150, this->width - 25,
                                               200, 250, "Back", 0, 0, 85, 170, 255, gameOverToMain));

    return true;
}

bool GameOverState::onExit()
{
    SoundManager::Instance()->haltMusic();
    SoundManager::Instance()->removeMusic("music");

    TextureManager::Instance()->removeColor("white");

    TextureManager::Instance()->removeFont("oxyReg30");

    TextureManager::Instance()->removeTexture("gameOverBackground");

    TextureManager::Instance()->removeTexture("Over");
    TextureManager::Instance()->removeTexture("Neu");
    TextureManager::Instance()->removeTexture("Back");

    InputManager::Instance()->reset();

    return true;
}
