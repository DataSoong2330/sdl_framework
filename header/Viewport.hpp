#ifndef __VIEWPORT__
#define __VIEWPORT__

#include <SDL2/SDL.h>
#include <string>

struct Viewport
{
    std::string viewportID = "none";
    std::string textureID = "none";
    std::string alignment = "none";
    SDL_Rect viewport = {0};
    float percentageX = 1.0f;
    float percentageY = 1.0f;
};

#endif // __VIEWPORT__
