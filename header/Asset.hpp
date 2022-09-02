#ifndef __ASSET__
#define __ASSET__

#include <SDL2/SDL.h>
#include <string>

#include "../header/CommonEnums.hpp"

struct Asset
{
    std::string fileName;
    std::string textureID;
    std::string viewportID;
    SDL_Rect destRect;
    SDL_Rect srcRect;
    double angle;
    SDL_Point center;
    FLIP sdlFlip;
};

#endif // __ASSET__
