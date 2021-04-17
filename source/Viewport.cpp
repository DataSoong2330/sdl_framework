#include "../header/Viewport.hpp"

Viewport::Viewport() : keyOfTexture("")
{
   this->viewport.x = 0;
   this->viewport.y = 0;
   this->viewport.w = 0;
   this->viewport.h = 0;
}

Viewport::Viewport(int x, int y, int w, int h, std::string key) : keyOfTexture(key)
{
    this->viewport.x = x;
    this->viewport.y = y;
    this->viewport.w = w;
    this->viewport.h = h;
}

Viewport::Viewport(SDL_Rect &viewportRect, std::string key) : keyOfTexture(key)
{
    this->viewport.x = viewportRect.x;
    this->viewport.y = viewportRect.y;
    this->viewport.w = viewportRect.w;
    this->viewport.h = viewportRect.h;
}