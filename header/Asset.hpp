#ifndef __ASSET__
#define __ASSET__

#include <SDL2/SDL.h>
#include <string>

#include "../header/CommonEnums.hpp"

struct ImageAsset
{
    std::string fileName = "none";
    std::string textureID = "none";
    std::string viewportID = "none";
    SDL_Rect destRect = {0};
    SDL_Rect srcRect = {0};
    double angle = 0.0d;
    SDL_Point center = {0};
    FLIP sdlFlip = FLIP::NONE;
};

struct TextAsset
{
    std::string text = "none";
    std::string fontName = "none";
    std::string textureID = "none";
    std::string viewportID = "none";
    std::string colorID = "none";
    SDL_Rect destRect = {0};
    SDL_Rect srcRect = {0};
    double angle = 0.0d;
    SDL_Point center = {0};
    FLIP sdlFlip = FLIP::NONE;
    TEXTQUALITY textQuality = TEXTQUALITY::SOLID;
};

#endif // __ASSET__
