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
            Logfile::Instance()->Textout("gameOverState", "music", "Paused");
            SoundManager::Instance()->pauseMusic();
        }
        else if(SoundManager::Instance()->isMusicPaused())
        {
            Logfile::Instance()->Textout("gameOverState", "music", "resume");
            SoundManager::Instance()->resumeMusic();
        }
        else if(!SoundManager::Instance()->isMusicPaused() &&
                !SoundManager::Instance()->isMusicPlaying())
        {
            Logfile::Instance()->Textout("gameOverState", "music", "stop");
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
    TextureManager::Instance()->drawTexture("Over", (this->mainViewport.w - TextureManager::Instance()->getWidthOfTexture("Over")) / 2, 50);

    // draws all the game objects
    if(this->gameObjects.size() > 0)
    {
        for(unsigned i = 0; i < this->gameObjects.size(); i++)
            this->gameObjects[i]->draw();
    }
}

bool GameOverState::onEnter()
{
    std::ifstream jsonFile("gameOverState.json");
    nlohmann::json name, text, color, ttf;

    this->functionMap["gameOverToMain"] = gameOverToMain;
    this->functionMap["restartPlay"] = restartPlay;

    if(jsonFile)
    {
        this->stateJson = nlohmann::json::parse(jsonFile);
    }
    else
    {
        Logfile::Instance()->Textout("GameState", "gameState.json", "failed to load");
    }

    InputManager::Instance()->reset();

    for(auto &item : this->stateJson["music"].items())
    {
        // get the path
        name = item.value()["path"];
        // get the id
        text = item.key();

        // load with path and key
        SoundManager::Instance()->load(name, text, SoundType::SOUND_MUSIC);
    }

    SoundManager::Instance()->playMusic("music", 0);

    // gets the current screen size, necessary for alignment of the buttons and textures
    SDL_GetWindowSize(Game::Instance()->getWindow(), &this->mainViewport.w, &this->mainViewport.h);
    this->mainViewport.x = this->mainViewport.y = 0;

    // loading of colors, fonts and textures
    for(auto &item : this->stateJson["color"].items())
    {
        // get the id
        name = item.key();

        TextureManager::Instance()->loadColor(name, item.value()["red"],
                                                    item.value()["green"],
                                                    item.value()["blue"],
                                                    item.value()["alpha"]);
    }

    for(auto &item : this->stateJson["ttf"].items())
    {
        // get the id
        name = item.value()["path"];
        // get the name
        text = item.key();

        // load the font with
        TextureManager::Instance()->loadFont(name, item.value()["size"], text);
    }

    for(auto &item : this->stateJson["assets"].items())
    {
        // get the path
        name = item.value()["path"];
        // get the id
        text = item.key();

        // load the image with path and id
        TextureManager::Instance()->loadImageTexture(name, text);
    }

    for(auto &item : this->stateJson["texttexture"].items())
    {
        // get the name
        name = item.key();
        // get the text
        text = item.value()["text"];
        // get the color
        color = item.value()["color"];
        // get the ttf
        ttf = item.value()["ttf"];

        TextureManager::Instance()->loadTextTexture(ttf, text, color, name, TextQuality::BLENDED);
    }

    for(auto &item : this->stateJson["buttons"].items())
    {
        name = item.value()["functionPointer"];
        text = item.key();

        int width = item.value()["width"];
        int height = item.value()["height"];
        int y1 = item.value()["ypos"];

        int x1 = (this->mainViewport.w - width) / 2;
        int x2 = x1 + width;

        int y2 = y1 + height;

        this->gameObjects.push_back(new MenuButton(x1, x2, y1, y2, text,
                                                    item.value()["angle"], item.value()["red"],
                                                    item.value()["green"], item.value()["blue"],
                                                    item.value()["alpha"], this->functionMap[name]));
    }

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
