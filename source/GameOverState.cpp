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
    /*TextureManager::Instance()->drawTexture("gameOverBackground",
                                            this->stateJson["assets"]["gameOverBackground"]["coordinates"]["xPos"],
                                            this->stateJson["assets"]["gameOverBackground"]["coordinates"]["yPos"],
                                            NULL, "screen");*/

    for(auto &item : this->assets)
    {
        TextureManager::Instance()->drawTexture(item.textureID, item.destRect.x, item.destRect.y, nullptr,
                                                item.viewportID, item.angle, &item.center,
                                                static_cast<SDL_RendererFlip>(item.sdlFlip));
    }

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
        Viewport tempViewport;

        tempViewport.viewportID = item.key();
        tempViewport.textureID = item.value()["texture"];
        tempViewport.alignment = item.value()["alignment"];

        tempViewport.percentageX = item.value()["relativeSizeX"];
        tempViewport.percentageY = item.value()["relativeSizeY"];
        tempViewport.viewport.w = screenWidth * tempViewport.percentageX;
        tempViewport.viewport.h = screenHeight * tempViewport.percentageY;

        if(tempViewport.alignment == "left")
        {
            tempViewport.viewport.x = 0;
            tempViewport.viewport.y = 0;
        }
        else
        {
            tempViewport.viewport.x = screenWidth - screenWidth * tempViewport.percentageX;
            tempViewport.viewport.y = 0;
        }

        TextureManager::Instance()->addViewport(tempViewport.viewport.x, tempViewport.viewport.y,
                                                tempViewport.viewport.w, tempViewport.viewport.h,
                                                tempViewport.viewportID);

        this->viewports.push_back(tempViewport);
    }

    for(auto &item : this->stateJson["buttons"].items())
    {
        Button values;

        values.textureID = item.key();
        values.viewportID = item.value()["viewport"];
        values.fontID = item.value()["ttf"];
        values.colorID = item.value()["color"];
        values.angle = item.value()["angle"];
        values.r = item.value()["red"];
        values.g = item.value()["green"];
        values.b = item.value()["blue"];
        values.a = item.value()["alpha"];
        values.func = this->functionMap[item.value()["functionPointer"]];
        values.sdlFlip = static_cast<FLIP>(item.value()["flip"]);

        TextureManager::Instance()->loadTextTexture(values.fontID, values.textureID, values.colorID,
                                                    values.textureID, TextQuality::BLENDED);

        TextureManager::Instance()->setBlendModeOfTexture(values.textureID, SDL_BLENDMODE_BLEND);

        this->texttextureKeys.push_back(values.textureID);

        SDL_Rect viewport = TextureManager::Instance()->getViewport(values.viewportID);

        percentageX = (float)item.value()["relativeSizeX"] / 100.0f;
        percentageY = (float)item.value()["relativeSizeY"] / 100.0f;

        int distX = viewport.w * percentageX;
        int distY = viewport.h * percentageY;

        int width = viewport.w - 2 * distX;
        int height = viewport.h * percentageX;

        values.y1 = distY * (this->gameObjects.size() + 1) + height * this->gameObjects.size();
        values.y2 = values.y1 + height;

        values.x1 = distX;
        values.x2 = values.x1 + width;

        this->gameObjects.push_back(new MenuButton(values));
    }

    for(auto &item : this->stateJson["assets"].items())
    {
        // current asset
        Asset curAsset;

        // get the path
        curAsset.fileName = item.value()["path"];
        // get the id
        curAsset.textureID = item.key();
        // get the viewportID
        curAsset.viewportID = item.value()["viewport"];

        SDL_Rect viewport = TextureManager::Instance()->getViewport(curAsset.viewportID);

        // get the dest rect
        curAsset.destRect.w = viewport.w;
        curAsset.destRect.h = viewport.h;
        curAsset.destRect.x = 0;
        curAsset.destRect.y = 0;

        // get the source rect
        curAsset.srcRect.w = item.value()["srcRect"]["width"];
        curAsset.srcRect.h = item.value()["srcRect"]["height"];
        curAsset.srcRect.x = item.value()["srcRect"]["xPos"];
        curAsset.srcRect.y = item.value()["srcRect"]["yPos"];

        // get the angle of the asset
        curAsset.angle = item.value()["angle"];

        // get the center of the asset
        curAsset.center.x = item.value()["centerPoint"]["xPos"];
        curAsset.center.y = item.value()["centerPoint"]["yPos"];

        curAsset.sdlFlip = static_cast<FLIP>(item.value()["flip"]);

        // load the image with path and id
        TextureManager::Instance()->loadImageTexture(curAsset.fileName, curAsset.textureID);

        this->assets.push_back(curAsset);
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

    for(auto &item : this->assets)
    {
        TextureManager::Instance()->removeTexture(item.textureID);
    }

    for(auto &item : this->texttextureKeys)
    {
        TextureManager::Instance()->removeTexture(item);
    }

    for(auto &item : this->viewports)
    {
        TextureManager::Instance()->removeViewport(item.viewportID);
    }

    for(auto &item : this->gameObjects)
    {
        delete item;
    }

    this->gameObjects.clear();

    InputManager::Instance()->reset();

    return true;
}
