#include "../header/Game.hpp"
#include "../header/Logfile.hpp"
#include "../header/TextureManager.hpp"

// initializes the static singleton
TextureManager* TextureManager::textureManager = 0;

// standard ctor is lazy, it does nothing
TextureManager::TextureManager()
{
}

// dtor calls the quit method
TextureManager::~TextureManager()
{
    // colors will cleared only
    this->colors.clear();

    // each font needs to be closed by SDL
    for(auto font : this->fonts)
        TTF_CloseFont(font.second);

    // clear the map
    this->fonts.clear();

    // textures need to be closed too
    for(auto texture : this->textures)
        texture.second->free();

    // clear the map
    this->textures.clear();

    // clear the viewports
    this->viewports.clear();

    // quit both TTF and IMG addons
    TTF_Quit();
    IMG_Quit();
}

// init with flags for image extensions like .jpeg or .png
bool TextureManager::initTextureManager(int imgFlags)
{
    bool success = false;

    if((IMG_Init(imgFlags) & imgFlags) && (TTF_Init() == 0))
    {
        Logfile::Instance()->Textout("TextureManager", "Init", "Image and TTF successfully initiated!");

        success = true;
    }
    else
    {
        Logfile::Instance()->Textout("TextureManager", "Init", SDL_GetError());
    }

    return success;
}

// loads a color and adds it to the map
bool TextureManager::loadColor(std::string id, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_Color tmpColor = { r, g, b, a };

    this->colors[id] = tmpColor;

    return true;
}

// loads a font from a .ttf file and saves it to the map
bool TextureManager::loadFont(std::string file, int pointSize, std::string id)
{
    bool success = false;

    TTF_Font* tmpFont = TTF_OpenFont(file.c_str(), pointSize);

    if(tmpFont != NULL)
    {
        this->fonts[id] = tmpFont;

        Logfile::Instance()->Textout("TextureManager", file, "loaded");

        success = true;
    }
    else
    {
        Logfile::Instance()->Textout("TextureManager", file, TTF_GetError());
    }

    return success;
}

// loads an image from a file and saves it in the map
bool TextureManager::loadImageTexture(std::string path, std::string textureID, bool colorKeying, std::string colorKey)
{
    bool success = false;
    Texture* texture = nullptr;

    if(textureID == "none")
    {
        this->textures[textureID] = new Texture();
    }
    else
    {
        texture = this->textures[textureID];

        if(texture == nullptr)
        {
            texture = new Texture();

            if(texture != nullptr)
            {
                success = texture->loadFromFile(path);
                this->textures[textureID] = texture;
            }
        }
        else
        {
            success = texture->loadFromFile(path);
        }
    }

    return success;
}

// loads a text with a font, a text and a color and saves it to the map
bool TextureManager::loadTextTexture(std::string fontID, std::string text, std::string colorID, std::string textureID, TextQuality textQuality)
{
    bool success = false;
    Texture* texture = nullptr;

    texture = this->textures[textureID];

    if(texture == nullptr)
    {
        texture = new Texture();

        if(texture != nullptr)
        {
            success = texture->loadFromRenderedText(this->fonts[fontID], text, this->colors[colorID], textQuality);
            this->textures[textureID] = texture;
        }
    }
    else
    {
        success = texture->loadFromRenderedText(this->fonts[fontID], text, this->colors[colorID], textQuality);
    }

    return success;
}

// adds a viewport to the map
void TextureManager::addViewport(SDL_Rect &viewport, std::string keyOfViewport)
{
    SDL_Rect view;
    view.x = viewport.x;
    view.y = viewport.y;
    view.w = viewport.w;
    view.h = viewport.h;
    this->viewports[keyOfViewport] = view;
}

// adds a viewport to the map
void TextureManager::addViewport(int x, int y, int w, int h, std::string keyOfViewport)
{
    SDL_Rect view;
    view.x = x;
    view.y = y;
    view.w = w;
    view.h = h;
    this->viewports[keyOfViewport] = view;
}

// changes the color of a texture, for example a text
void TextureManager::setColorOfTexture(std::string textureID, std::string colorID)
{
    this->textures[textureID]->setColor(this->colors[colorID]);
}

// changes the blend mode of a texture
void TextureManager::setBlendModeOfTexture(std::string textureID, SDL_BlendMode blending)
{
    this->textures[textureID]->setBlendMode(blending);
}

// changes the transparency of a texture
void TextureManager::setAlphaOfTexture(std::string textureID, Uint8 alpha)
{
    this->textures[textureID]->setAlpha(alpha);
}

// renders a texture to the screen
void TextureManager::drawTexture(std::string textureID, int x, int y, SDL_Rect* clip, std::string viewportID, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    this->textures[textureID]->render(x, y, clip, this->viewports[viewportID], angle, center, flip);
}

// renders a box to the screen
void TextureManager::drawBox(std::string viewportID, int x1, int x2, int y1, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_RenderSetViewport(Game::Instance()->getRenderer(), &this->viewports[viewportID]);
    boxRGBA(Game::Instance()->getRenderer(), x1, y1, x2, y2, r, g, b, a);
}

// renders a circle to the screen
void TextureManager::drawCircle(std::string viewportID, int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_RenderSetViewport(Game::Instance()->getRenderer(), &this->viewports[viewportID]);
    circleRGBA(Game::Instance()->getRenderer(), x, y, radius, r, g, b, a);
}

// renders an ellipse to the screen
void TextureManager::drawEllipse(std::string viewportID, int x, int y, int rx, int ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_RenderSetViewport(Game::Instance()->getRenderer(), &this->viewports[viewportID]);
    ellipseRGBA(Game::Instance()->getRenderer(), x, y, rx, ry, r, g, b, a);
}

// removes a texture from the manager
void TextureManager::removeTexture(std::string id)
{
    this->textures[id]->free();
    this->textures.erase(id);
}

// removes a viewport from the manager
void TextureManager::removeViewport(std::string id)
{
    this->viewports.erase(id);
}

// removes a font from the manager
void TextureManager::removeFont(std::string id)
{
    TTF_CloseFont(this->fonts[id]);
    this->fonts.erase(id);
}
