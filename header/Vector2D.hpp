#ifndef __VECTOR2D__
#define __VECTOR2D__

#include <math.h>

class Vector2D
{
    public:
        // standard ctor
        Vector2D();
        // ctor initializes the position variables x and y
        Vector2D(float x, float y);
        // lazy dtor does nothing
        ~Vector2D() {}

        // get the x and y values
        inline const float getXPos() const { return this->xPos; }
        inline const float getYPos() const { return this->yPos; }

        // calculates the length of the vector
        const float calcLength() const;

        // sets the vector to a new position
        void setXPos(float newXPos) { this->xPos = newXPos; }
        void setYPos(float newYPos) { this->yPos = newYPos; }

    protected:
    private:
        // the x and y variables for the position
        float xPos;
        float yPos;
};

#endif // __VECTOR2D__
