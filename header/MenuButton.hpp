#ifndef __MENUBUTTON__
#define __MENUBUTTON__

#include <functional>

#include "../header/CommonEnums.hpp"
#include "../header/GameObject.hpp"

struct Button
{
    int x1 = 0, x2 = 0, y1 = 0, y2 = 0, x = 0, y = 0;
    std::string textureID = "none", viewportID = "none", fontID = "none", colorID = "none";
    double angle = 0.0d;
    unsigned r = 0, g = 85, b = 170, a = 255;
    std::function<void()> func = nullptr;
    SDL_Rect destRect = {0};
    SDL_Rect srcRect = {0};
    SDL_Point center = {0};
    FLIP sdlFlip = FLIP::NONE;
    TEXTQUALITY textQuality = TEXTQUALITY::SOLID;
};

class MenuButton : public GameObject
{
    public:
        // standard ctor inits everything with 0
        MenuButton();
        // user defined ctor for init with external values
        MenuButton(Button values);
        // dtor frees memory
        ~MenuButton();

        // draw the object
        void draw();
        // update the object
        void update();

    protected:
    private:
        // function pointer to call a specific state function
        std::function<void()> stateFunc;

        // coordinates for the box
        int x1, x2, y1, y2, x, y;

        // the texture id of the used text
        // necessary for the texture manager
        std::string textureID;
        // the viewport id
        std::string viewportID;
        // the font id
        std::string fontID;

        // the destination rectangle
        SDL_Rect destRect;
        // the source rectangle
        SDL_Rect srcRect;
        // the center point of the texture
        SDL_Point center;
        // is the texture flipped? and if so, how?
        FLIP sdlFlip;

        bool released;
};

#endif // __MENUBUTTON__
