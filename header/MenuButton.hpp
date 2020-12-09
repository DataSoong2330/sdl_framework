#ifndef __MENUBUTTON__
#define __MENUBUTTON__

#include <functional>

#include "../header/GameObject.hpp"

class MenuButton : public GameObject
{
    public:
        // standard ctor inits everything with 0
        MenuButton();
        // user defined ctor for init with external values
        MenuButton(int x1, int x2, int y1, int y2, std::string texture, double angle = 0,
                   unsigned r = 0, unsigned g = 85, unsigned b = 170, unsigned a = 255,
                   std::function<void()> stateFunc = nullptr);
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

        bool released;
};

#endif // __MENUBUTTON__
