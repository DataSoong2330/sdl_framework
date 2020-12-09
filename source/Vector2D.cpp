#include "../header/Vector2D.hpp"

// ctor for init the pos variables
Vector2D::Vector2D()
{
    this->xPos = 0.0f;
    this->yPos = 0.0f;
}

// ctor for user defined variables
Vector2D::Vector2D(float x, float y)
{
    this->xPos = x;
    this->yPos = y;
}

// calculate the length of the vector
const float Vector2D::calcLength() const
{
    return (sqrt(this->xPos * this->xPos + this->yPos * this->yPos));
}
