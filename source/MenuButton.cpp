#include "../header/InputManager.hpp"
#include "../header/MenuButton.hpp"
#include "../header/TextureManager.hpp"

MenuButton::MenuButton() : GameObject(), stateFunc(nullptr), x1(0), x2(0), y1(0), y2(0), textureID(""), released(true)
{
}

MenuButton::MenuButton(Button values) : GameObject(Vector2D(0,0), 0, 0, values.angle, values.r, values.g, values.b,
                                        values.a), stateFunc(values.func), x1(values.x1), x2(values.x2), y1(values.y1),
                                        y2(values.y2), x(values.x), y(values.y), textureID(values.textureID),
                                        viewportID(values.viewportID), fontID(values.fontID),
                                        sdlFlip(values.sdlFlip), released(true)
{
    this->destRect.x = values.destRect.x;
    this->destRect.y = values.destRect.y;
    this->destRect.w = values.destRect.w;
    this->destRect.h = values.destRect.h;

    this->srcRect.x = values.srcRect.x;
    this->srcRect.y = values.srcRect.y;
    this->srcRect.w = values.srcRect.w;
    this->srcRect.h = values.srcRect.h;

    this->center.x = values.center.x;
    this->center.y = values.center.y;

    int textureWidth = TextureManager::Instance()->getWidthOfTexture(this->textureID);
    int textureHeight = TextureManager::Instance()->getHeightOfTexture(this->textureID);

    this->width = this->x2 - this->x1;
    this->height = this->y2 - this->y1;

    int tempWidth = 0, tempHeight = 0;

    tempWidth = this->width - textureWidth;
    this->x = this->x1 + (tempWidth / 2);

    tempHeight = this->height - textureHeight;
    this->y = this->y1 + (tempHeight / 2);
}

MenuButton::~MenuButton()
{
}

void MenuButton::draw()
{
    TextureManager::Instance()->drawBox(this->viewportID, this->x1, this->x2, this->y1, this->y2, this->r, this->g, this->b, this->a);

    TextureManager::Instance()->drawTexture(this->textureID, this->x, this->y, nullptr, this->viewportID, this->angle, nullptr, static_cast<SDL_RendererFlip>(this->sdlFlip));
}

void MenuButton::update()
{
    Vector2D* mouse = InputManager::Instance()->getMousePosition();

    if(this->x1 <= mouse->getXPos() && mouse->getXPos() <= this->x2
       && this->y1 <= mouse->getYPos() && mouse->getYPos() <= this->y2)
    {
        this->a = 127;

        if(InputManager::Instance()->getMouseButtonState(mouse_buttons::LEFT) && this->released)
        {
            TextureManager::Instance()->setAlphaOfTexture(this->textureID, 127);

            if(this->stateFunc != 0)
                this->stateFunc();

            this->released = false;
        }
        else if(!InputManager::Instance()->getMouseButtonState(mouse_buttons::LEFT))
        {
            TextureManager::Instance()->setAlphaOfTexture(this->textureID, 255);

            this->released = true;
        }
    }
    else
    {
        this->a = 255;
    }
}
