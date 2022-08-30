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
            Logfile::Instance()->Textout("gameOverState", "background", "Paused");
            SoundManager::Instance()->pauseMusic();
        }
        else if(SoundManager::Instance()->isMusicPaused())
        {
            Logfile::Instance()->Textout("gameOverState", "background", "resume");
            SoundManager::Instance()->resumeMusic();
        }
        else if(!SoundManager::Instance()->isMusicPaused() &&
                !SoundManager::Instance()->isMusicPlaying())
        {
            Logfile::Instance()->Textout("gameOverState", "background", "stop");
            SoundManager::Instance()->playMusic("background", 0);
        }
    }
    if(InputManager::Instance()->isKeyPressed(SDL_SCANCODE_ESCAPE))
    {
        if(SoundManager::Instance()->isMusicPlaying() ||
           SoundManager::Instance()->isMusicPaused())
            SoundManager::Instance()->haltMusic();
    }

    // draws all the game objects
    if(this->gameObjects.size() > 0)
    {
        for(unsigned i = 0; i < this->gameObjects.size(); i++)
            this->gameObjects[i]->update();
    }
}

void GameOverState::render()
{
    // draws the textures with their id and a coordinate
    TextureManager::Instance()->drawTexture("gameOverBackground",
                                            this->stateJson["assets"]["gameOverBackground"]["coordinates"]["xPos"],
                                            this->stateJson["assets"]["gameOverBackground"]["coordinates"]["yPos"],
                                            NULL, "screen");
    TextureManager::Instance()->drawTexture("Over", (this->screenSize.w - TextureManager::Instance()->getWidthOfTexture("Over")) / 2, 50,
                                            NULL, "screen");

    // draws all the game objects
    if(this->gameObjects.size() > 0)
    {
        for(unsigned i = 0; i < this->gameObjects.size(); i++)
            this->gameObjects[i]->draw();
    }
}

bool GameOverState::onEnter()
{
    std::ifstream jsonFile("states/gameOverState.json");
    nlohmann::json name, text, color, ttf;

    this->functionMap["gameOverToMain"] = gameOverToMain;
    this->functionMap["restartPlay"] = restartPlay;

    // gets the current screen size, necessary for alignment of the buttons and textures
    SDL_GetWindowSize(Game::Instance()->getWindow(), &this->screenSize.w, &this->screenSize.h);
    this->screenSize.x = this->screenSize.y = 0;

    if(jsonFile)
    {
        this->stateJson = nlohmann::ordered_json::parse(jsonFile);
    }
    else
    {
        Logfile::Instance()->Textout("GameState", "states/gameOverState.json", "failed to load");
    }

    InputManager::Instance()->reset();

    for(auto &item : this->stateJson["music"].items())
    {
        // get the path
        name = item.value()["path"];
        // get the id
        text = item.key();

        this->musicKeys.push_back(text);

        // load with path and key
        SoundManager::Instance()->load(name, text, SoundType::SOUND_MUSIC);
    }

    SoundManager::Instance()->playMusic("background", 0);

    for(auto &item : this->stateJson["sound"].items())
    {
        // get the path
        name = item.value()["path"];
        // get the id
        text = item.key();

        this->soundKeys.push_back(text);

        // load with path and key
        SoundManager::Instance()->load(name, text, SoundType::SOUND_SFX);
    }

    // loading of colors, fonts and textures
    for(auto &item : this->stateJson["color"].items())
    {
        // get the id
        name = item.key();

        this->colorKeys.push_back(name);

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

        this->ttfKeys.push_back(text);

        // load the font with
        TextureManager::Instance()->loadFont(name, item.value()["size"], text);
    }

    for(auto &item : this->stateJson["assets"].items())
    {
        // get the path
        name = item.value()["path"];
        // get the id
        text = item.key();

        this->assetKeys.push_back(text);

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

        this->texttextureKeys.push_back(name);

        TextureManager::Instance()->loadTextTexture(ttf, text, color, name, TextQuality::BLENDED);
    }

    int screenWidth = 0;
    int screenHeight = 0;
    SDL_GetWindowSize(Game::Instance()->getWindow(), &screenWidth, &screenHeight);
    float percentageX = 0.0f;
    float percentageY = 0.0f;

    for(auto &item : this->stateJson["viewports"].items())
    {
        int viewportWidth = 0;
        int viewportHeight = 0;

        text = item.key();
        name = item.value()["texture"];

        this->viewportKeys.push_back(text);

        percentageX = (float)(item.value()["relativeSize"]) / 100.0f;
        viewportWidth = screenWidth * percentageX;
        viewportHeight = screenHeight;

        TextureManager::Instance()->addViewport(item.value()["xPos"], item.value()["yPos"],
                                                    viewportWidth, viewportHeight, text);
    }

    for(auto &item : this->stateJson["buttons"].items())
    {
        name = item.value()["viewport"];

        SDL_Rect viewport = TextureManager::Instance()->getViewport(name);

        percentageX = (float)item.value()["relativeSizeX"] / 100.0f;
        percentageY = (float)item.value()["relativeSizeY"] / 100.0f;

        int distX = viewport.w * percentageX;
        int distY = viewport.h * percentageY;

        int width = viewport.w - 2 * distX;
        int height = viewport.h * percentageX;

        int y1 = distY * (this->gameObjects.size() + 1) + height * this->gameObjects.size();
        int y2 = y1 + height;

        int x1 = distX;
        int x2 = x1 + width;

        name = item.value()["functionPointer"];
        text = item.key();

        this->gameObjects.push_back(new MenuButton( x1, x2, y1, y2, text,
                                                    item.value()["angle"], item.value()["red"],
                                                    item.value()["green"], item.value()["blue"],
                                                    item.value()["alpha"], this->functionMap[name]));
    }

    return true;
}

bool GameOverState::onExit()
{
    SoundManager::Instance()->haltMusic();

    for(auto &item : this->musicKeys)
    {
        SoundManager::Instance()->removeMusic(item);
    }

    for(auto &item : this->colorKeys)
    {
        TextureManager::Instance()->removeColor(item);
    }

    for(auto &item : this->ttfKeys)
    {
        TextureManager::Instance()->removeFont(item);
    }

    for(auto &item : this->assetKeys)
    {
        TextureManager::Instance()->removeTexture(item);
    }

    for(auto &item : this->texttextureKeys)
    {
        TextureManager::Instance()->removeTexture(item);
    }

    for(auto &item : this->viewportKeys)
    {
        TextureManager::Instance()->removeViewport(item);
    }

    for(auto &item : this->gameObjects)
    {
        delete item;
    }

    this->gameObjects.clear();

    InputManager::Instance()->reset();

    return true;
}
