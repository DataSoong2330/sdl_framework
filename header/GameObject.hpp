#ifndef __GAMEOBJECT__
#define __GAMEOBJECT__

#include <string>

#include "../header/Vector2D.hpp"

class GameObject
{
    public:
        GameObject() : position(0,0), width(0), height(0), angle(0), r(0), g(0), b(0), a(255)
        {
        }

        GameObject(Vector2D position, int width, int height, double angle,
                   unsigned r, unsigned g, unsigned b, unsigned a) : position(position.getXPos(), position.getYPos()),
                   width(width), height(height), angle(angle), r(r), g(g), b(b), a(a)
        {
        }

        // lazy virtual dtor -> does nothing
        virtual ~GameObject() {}

        // draw the object
        virtual void draw() = 0;

        // update the object
        virtual void update() = 0;

        // get the position of the game object
        inline const Vector2D& getPosition() const { return this->position; }

        // get the dimensions of the game object
        inline const int getHeight() const { return this->height; }
        inline const int getWidth() const { return this->width; }

    protected:
        // position of the game object
        Vector2D position;

        // two dimensions for height and width
        int width;
        int height;

        // the angle for rendering
        double angle;

        // the red, green, blue and alpha values
        unsigned r, g, b, a;

    private:
};

#endif // __GAMEOBJECT__
