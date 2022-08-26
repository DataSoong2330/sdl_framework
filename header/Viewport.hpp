#ifndef __VIEWPORT__
#define __VIEWPORT__

#include <SDL2/SDL.h>
#include <string>

class Viewport
{
    public:
        // std ctor
        Viewport();
        // ctor takes coordinates, width, height and key of texture
        Viewport(int x, int y, int w, int h, std::string key);
        // cotr takes SDL_Rec& and key of texture
        Viewport(SDL_Rect &viewportRect, std::string key);

        const SDL_Rect* getViewportRect() const { return &this->viewport; }
        std::string getKeyOfTexture() { return this->keyOfTexture; }

    protected:
    private:
        SDL_Rect viewport;

        std::string keyOfTexture;
};

#endif // __VIEWPORT
