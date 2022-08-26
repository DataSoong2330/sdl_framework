#include "../header/Game.hpp"
#include "../header/Logfile.hpp"
#include "../header/Texture.hpp"

// ctor initializes members
Texture::Texture()
{
    this->texture = nullptr;
    this->height = 0;
    this->width = 0;
}

// dtor for freeing the memory
Texture::~Texture()
{
    // free the current texture
    this->free();
}

// frees the current texture
void Texture::free()
{
    // free the texture if it exists and sets everything to 0
    if(this->texture != NULL)
    {
        SDL_DestroyTexture(this->texture);
        this->texture = NULL;
        this->height = 0;
        this->width = 0;
    }
}

// loads an image from the path
bool Texture::loadFromFile(std::string path, bool colorKeying, SDL_Color colorKey)
{
    // delete old texture
    this->free();

    // load the file to a temporary surface
    SDL_Surface* tempSurface = IMG_Load(path.c_str());

    if(tempSurface != NULL)
    {
        // if the surface could be created
        // color key image
        if(colorKeying)
            SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, colorKey.r, colorKey.g, colorKey.b));

        // create texture from surface
        this->texture = SDL_CreateTextureFromSurface(Game::Instance()->getRenderer(), tempSurface);

        if(this->texture != NULL)
        {
            // if the texture was created successfully
            this->height = tempSurface->h;
            this->width = tempSurface->w;

            Logfile::Instance()->Textout("Texture", path, "loaded");
        }
        else
        {
            // if an error occured
            Logfile::Instance()->Textout("Texture", path, SDL_GetError());
        }

        // free the memory of the surface
        SDL_FreeSurface(tempSurface);
    }
    else
    {
        // if the surface could not be created
        Logfile::Instance()->Textout("Texture", path, SDL_GetError());
    }

    return (this->texture != NULL);
}

// loads a texture from a text
bool Texture::loadFromRenderedText(TTF_Font* font, std::string text, SDL_Color textColor, TextQuality textQuality)
{
    // delete the old texture
    this->free();

    SDL_Surface* tempSurface = NULL;

    // render the text surface
    switch(textQuality)
    {
        case TextQuality::SOLID:    tempSurface = TTF_RenderUTF8_Solid(font, text.c_str(), textColor);
                                    break;
        case TextQuality::SHADED:   tempSurface = TTF_RenderUTF8_Shaded(font, text.c_str(), textColor, { 255, 255, 255, 255 });
                                    break;
        case TextQuality::BLENDED:  tempSurface = TTF_RenderUTF8_Blended(font, text.c_str(), textColor);
                                    break;
        default:                    tempSurface = TTF_RenderUTF8_Solid(font, text.c_str(), textColor);
                                    break;
    }

    if(tempSurface != NULL)
    {
        // if the surface was created successfully
        this->texture = SDL_CreateTextureFromSurface(Game::Instance()->getRenderer(), tempSurface);

        if(this->texture != NULL)
        {
            // if the texture was created successfully set the dimensions
            this->height = tempSurface->h;
            this->width = tempSurface->w;

            Logfile::Instance()->Textout("TTF", text, "loaded");
        }
        else
        {
            // if an error occured
            Logfile::Instance()->Textout("TTF", text, SDL_GetError());
        }

        // free the memory of the surface
        SDL_FreeSurface(tempSurface);
    }
    else
    {
        // if an error occured
        Logfile::Instance()->Textout("TTF", text, TTF_GetError());
    }

    return (this->texture != NULL);
}

// change the color of a texture
void Texture::setColor(SDL_Color color)
{
    // modulate the color of the texture
    SDL_SetTextureColorMod(this->texture, color.r, color.g, color.b);
}

// changes the blend mode
void Texture::setBlendMode(SDL_BlendMode blending)
{
    // set the blending value
    SDL_SetTextureBlendMode(this->texture, blending);
}

// change the transparency
void Texture::setAlpha(Uint8 alpha)
{
    // modulate the alpha of the texture
    SDL_SetTextureAlphaMod(this->texture, alpha);
}

// renders the texture to the screen
void Texture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    // sets the quad where the texture is rendered to
    SDL_Rect renderQuad = { x, y, this->width, this->height };

    // if the texture shall be clipped
    if(clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    // the render function
    SDL_RenderCopyEx(Game::Instance()->getRenderer(), this->texture, clip, &renderQuad, angle, center, flip);
}

// render the port to the screen
void Texture::renderPort(const SDL_Rect* portDimension)
{
    SDL_RenderSetViewport(Game::Instance()->getRenderer(), portDimension);
    SDL_RenderCopy(Game::Instance()->getRenderer(), this->texture, nullptr, nullptr);
}
