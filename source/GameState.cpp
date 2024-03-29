#include "../header/Game.hpp"
#include "../header/GameState.hpp"
#include "../header/GameStateMachine.hpp"
#include "../header/InputManager.hpp"
#include "../header/Logfile.hpp"
#include "../header/SoundManager.hpp"
#include "../header/TextureManager.hpp"

GameState::GameState(GameStateMachine& stateMachine) : gameStateMachine(&stateMachine)
{
}

bool GameState::update()
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

    // updates all the game objects
    if(this->gameObjects.size() > 0)
    {
        for(unsigned i = 0; i < this->gameObjects.size(); i++)
            this->gameObjects[i]->update();
    }

    return true;
}

void GameState::render()
{
    // draws the textures with their id and a coordinate
    for(auto &item : this->assets)
    {
        TextureManager::Instance()->drawTexture(item.textureID, item.destRect.x, item.destRect.y, nullptr,
                                                item.viewportID, item.angle, &item.center,
                                                static_cast<SDL_RendererFlip>(item.sdlFlip));
    }

    for(auto &item : this->textAssets)
    {
        TextureManager::Instance()->drawTexture(item.textureID, item.destRect.x, item.destRect.y, nullptr,
                                                item.viewportID, item.angle, nullptr,
                                                static_cast<SDL_RendererFlip>(item.sdlFlip));
    }

    // draws all the game objects
    if(this->gameObjects.size() > 0)
    {
        for(const auto &item : this->gameObjects)
        {
            item->draw();
        }
    }
}

bool GameState::handleEvents()
{
    return true;
}

bool GameState::onEnter(std::string fileName)
{
    bool success = false;
    std::ifstream jsonFile(fileName);

    if(jsonFile)
    {
        this->stateJson = nlohmann::ordered_json::parse(jsonFile);

        this->loadMusic();
        this->loadSound();
        this->loadColor();
        this->loadTTF();
        this->loadViewports();
        this->loadTextTextures();
        this->loadButtons();
        this->loadAssets();

        success = true;
    }
    else
    {
        Logfile::Instance()->Textout("GameState", "failed to load", fileName);
    }

    InputManager::Instance()->reset();
    SoundManager::Instance()->playMusic("background", 0);

    return success;
}

bool GameState::onExit()
{
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

    for(auto &item : this->textAssets)
    {
        TextureManager::Instance()->removeTexture(item.textureID);
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

void GameState::requestStackPush(States stateID)
{
    this->gameStateMachine->pushState(stateID);
}

void GameState::requestStackPop()
{
    this->gameStateMachine->popState();
}

void GameState::requestStackClear()
{
    this->gameStateMachine->clearStates();
}

void GameState::loadMusic()
{
    for(auto &item : this->stateJson["music"].items())
    {
        // get the path
        this->name = item.value()["path"];
        // get the id
        this->text = item.key();

        this->musicKeys.push_back(text);

        // load with path and key
        SoundManager::Instance()->load(name, text, SoundType::SOUND_MUSIC);
    }
}

void GameState::loadSound()
{
    for(auto &item : this->stateJson["sound"].items())
    {
        // get the path
        this->name = item.value()["path"];
        // get the id
        this->text = item.key();

        this->soundKeys.push_back(text);

        // load with path and key
        SoundManager::Instance()->load(name, text, SoundType::SOUND_SFX);
    }
}

void GameState::loadColor()
{
    // loading of colors, fonts and textures
    for(auto &item : this->stateJson["colors"].items())
    {
        // get the id
        this->name = item.key();

        this->colorKeys.push_back(name);

        TextureManager::Instance()->loadColor(name, item.value()["red"],
                                                    item.value()["green"],
                                                    item.value()["blue"],
                                                    item.value()["alpha"]);
    }
}

void GameState::loadTTF()
{
    for(auto &item : this->stateJson["ttf"].items())
    {
        // get the id
        this->name = item.value()["path"];
        // get the name
        this->text = item.key();

        this->ttfKeys.push_back(text);

        // load the font with
        TextureManager::Instance()->loadFont(name, item.value()["size"], text);
    }
}

void GameState::loadViewports()
{
    // gets the current screen size, necessary for alignment of the buttons and textures
    SDL_GetWindowSize(Game::Instance()->getWindow(), &this->screenSize.w, &this->screenSize.h);
    this->screenSize.x = this->screenSize.y = 0;

    for(auto &item : this->stateJson["viewports"].items())
    {
        Viewport viewport;

        viewport.viewportID = item.key();
        viewport.textureID = item.value()["texture"];
        viewport.alignment = item.value()["alignment"];

        viewport.percentageX = item.value()["relativeSizeX"];
        viewport.percentageY = item.value()["relativeSizeY"];
        viewport.viewport.w = screenSize.w * viewport.percentageX;
        viewport.viewport.h = screenSize.h * viewport.percentageY;

        if(viewport.alignment == "left")
        {
            viewport.viewport.x = 0;
            viewport.viewport.y = 0;
        }
        else
        {
            viewport.viewport.x = screenSize.w - screenSize.w * viewport.percentageX;
            viewport.viewport.y = 0;
        }

        TextureManager::Instance()->addViewport(viewport.viewport.x, viewport.viewport.y,
                                                viewport.viewport.w, viewport.viewport.h,
                                                viewport.viewportID);

        this->viewports.push_back(viewport);
    }
}

void GameState::loadTextTextures()
{
    for(auto &item : this->stateJson["texttexture"].items())
    {
        TextAsset textAsset;

        // get the name
        textAsset.textureID = item.key();
        // get the text
        textAsset.text = item.value()["text"];
        // get the color
        textAsset.colorID = item.value()["colorID"];
        // get the fontID
        textAsset.fontName = item.value()["fontID"];
        // get the texture quality
        textAsset.textQuality = static_cast<TEXTQUALITY>(item.value()["quality"]);
        // get the viewport
        textAsset.viewportID = item.value()["viewport"];

        SDL_Rect viewport = TextureManager::Instance()->getViewport(textAsset.viewportID);

        TextureManager::Instance()->loadTextTexture(textAsset.fontName, textAsset.text, textAsset.colorID,
                                                    textAsset.textureID,
                                                    static_cast<TextQuality>(textAsset.textQuality));

        textAsset.destRect.w = TextureManager::Instance()->getWidthOfTexture(textAsset.textureID);
        textAsset.destRect.h = TextureManager::Instance()->getHeightOfTexture(textAsset.textureID);

        textAsset.destRect.x = (viewport.w - textAsset.destRect.w) / 2;
        textAsset.destRect.y = (viewport.h - textAsset.destRect.h) / 2;

        this->textAssets.push_back(textAsset);
    }
}

void GameState::loadButtons()
{
    for(auto &item : this->stateJson["buttons"].items())
    {
        Button values;
        TextAsset textAsset;

        values.textureID = textAsset.textureID = item.key();
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
        values.textQuality = static_cast<TEXTQUALITY>(item.value()["quality"]);

        TextureManager::Instance()->loadTextTexture(values.fontID, values.textureID, values.colorID,
                                                    values.textureID,
                                                    static_cast<TextQuality>(values.textQuality));

        TextureManager::Instance()->setBlendModeOfTexture(values.textureID, SDL_BLENDMODE_BLEND);

        this->textAssets.push_back(textAsset);

        SDL_Rect viewport = TextureManager::Instance()->getViewport(values.viewportID);

        float percentageX = (float)item.value()["relativeSizeX"] / 100.0f;
        float percentageY = (float)item.value()["relativeSizeY"] / 100.0f;

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
}

void GameState::loadAssets()
{
    for(auto &item : this->stateJson["assets"].items())
    {
        // current asset
        ImageAsset imageAsset;

        // get the path
        imageAsset.fileName = item.value()["path"];
        // get the id
        imageAsset.textureID = item.key();
        // get the viewportID
        imageAsset.viewportID = item.value()["viewport"];

        SDL_Rect viewport = TextureManager::Instance()->getViewport(imageAsset.viewportID);

        // get the dest rect
        imageAsset.destRect.w = viewport.w;
        imageAsset.destRect.h = viewport.h;
        imageAsset.destRect.x = 0;
        imageAsset.destRect.y = 0;

        // get the source rect
        imageAsset.srcRect.w = item.value()["srcRect"]["width"];
        imageAsset.srcRect.h = item.value()["srcRect"]["height"];
        imageAsset.srcRect.x = item.value()["srcRect"]["xPos"];
        imageAsset.srcRect.y = item.value()["srcRect"]["yPos"];

        // get the angle of the asset
        imageAsset.angle = item.value()["angle"];

        // get the center of the asset
        imageAsset.center.x = item.value()["centerPoint"]["xPos"];
        imageAsset.center.y = item.value()["centerPoint"]["yPos"];

        imageAsset.sdlFlip = static_cast<FLIP>(item.value()["flip"]);

        // load the image with path and id
        TextureManager::Instance()->loadImageTexture(imageAsset.fileName, imageAsset.textureID);

        this->assets.push_back(imageAsset);
    }
}
