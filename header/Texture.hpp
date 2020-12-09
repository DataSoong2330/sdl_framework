#ifndef __TEXTURE__
#define __TEXTURE__

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>

// enum for the three text qualities
// solid = low, shaded = medium, blended = high
enum TextQuality { SOLID, SHADED, BLENDED };

class Texture
{
    public:
        // ctor initializes members
        Texture();

        // dtor for freeing the memory
        ~Texture();

        // frees the current texture
        void free();

        // loads a image from a path
        // colorKeying deletes a defined color from the texture if wished
        bool loadFromFile(std::string path, bool colorKeying = false, SDL_Color colorKey = { 0, 255, 255, 255 });
        // loads a text as texture using a font
        bool loadFromRenderedText(TTF_Font* font, std::string text, SDL_Color textColor, TextQuality textQuality = TextQuality::SOLID);

        // modulate the color of the texture
        void setColor(SDL_Color color);
        // set the blending function
        void setBlendMode(SDL_BlendMode blending);
        // modulate the alpha of the texture
        void setAlpha(Uint8 alpha);

        // the render function for the texture
        // x and y for upper left coordinates
        // clip means to render not the complete texture, not the hole thingy
        // angle for rotation, center is the middle of the render point
        // flip HORIZONZAL or VERTICAL
        void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

        // get the dimensions
        inline const int& getHeight() const { return this->height; }
        inline const int& getWidth() const { return this->width; }

    protected:
    private:
        // the actual SDL_Texture
        SDL_Texture* texture;

        // the dimensions of the texture
        int height;
        int width;
};

#endif // __TEXTURE__
