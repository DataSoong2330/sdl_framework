#ifndef __TEXTUREMANAGER__
#define __TEXTUREMANAGER__

#include <map>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "../header/Texture.hpp"

class TextureManager
{
    public:
        // singleton instance
        static TextureManager* Instance()
        {
            if(textureManager == NULL)
                textureManager = new TextureManager();

            return textureManager;
        }

        // init the manager with the image flags
        // PNG, JPG, WEBG, etc...
        bool initTextureManager(int imgFlags);

        // loads a color and saves it
        bool loadColor(std::string id, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
        // loads a font and saves it
        bool loadFont(std::string file, int pointSize, std::string id);
        // loads an image from a file and saves it
        bool loadImageTexture(std::string path, std::string textureID, bool colorKeying = false, std::string colorKey = "cyan");
        // loads a text and saves it
        bool loadTextTexture(std::string fontID, std::string text, std::string colorID, std::string textureID, TextQuality textQuality = TextQuality::SOLID);

        // changes the color of a texture
        void setColorOfTexture(std::string textureID, std::string colorID);
        // sets the blending mode to NONE, ALPHA, ADD or MOD
        void setBlendModeOfTexture(std::string textureID, SDL_BlendMode blending);
        // changes the transparency of a texture
        void setAlphaOfTexture(std::string textureID, Uint8 alpha);

        // get the height of a texture
        inline const int getHeightOfTexture(std::string textureID) { return this->textures[textureID]->getHeight(); }
        // get the width of a texture
        inline const int getWidthOfTexture(std::string textureID) { return this->textures[textureID]->getWidth(); }

        // draws a texture to the screen
        void drawTexture(std::string textureID, int x, int y, SDL_Rect* clip = NULL, double angle = 0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
        // draws a box to the screen
        void drawBox(int x1, int x2, int y1, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
        // draws a circle to the screen
        void drawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
        // draws a ellipse to the screen
        void drawEllipse(int x, int y, int rx, int ry, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

        // removes a texture from the manager
        void removeTexture(std::string id);
        // removes a color from the manager
        inline void removeColor(std::string id) { this->colors.erase(id); }
        // removes a font from the manager
        void removeFont(std::string id);

        // frees the memory used by the textures and fonts
        void freeMemory() { delete textureManager; textureManager = nullptr; }

    protected:
    private:
        // private ctor and dtor for singleton
        TextureManager();
        ~TextureManager();

        // the singleton
        static TextureManager* textureManager;

        // the maps store textures, colors and fonts
        std::map<std::string, Texture*> textures;
        std::map<std::string, SDL_Color> colors;
        std::map<std::string, TTF_Font*> fonts;
};

#endif // __TEXTUREMANAGER__
