#include "../header/InputManager.hpp"
#include "../header/MenuButton.hpp"
#include "../header/TextureManager.hpp"

MenuButton::MenuButton() : GameObject(), stateFunc(0), x1(0), x2(0), y1(0), y2(0), textureID(""), released(true)
{

}

MenuButton::MenuButton( int x1, int x2, int y1, int y2, std::string texture,
            double angle, unsigned r, unsigned g, unsigned b, unsigned a,
            std::function<void()> stateFunc) : GameObject(Vector2D(0, 0), width, height,
            angle, r, g, b, a), stateFunc(stateFunc), x1(x1), x2(x2), y1(y1), y2(y2),
            textureID(texture), released(true)
{
    TextureManager::Instance()->setBlendModeOfTexture(this->textureID, SDL_BLENDMODE_BLEND);

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
    TextureManager::Instance()->drawBox("menu", this->x1, this->x2, this->y1, this->y2,
                                        this->r, this->g, this->b, this->a);

    TextureManager::Instance()->drawTexture(this->textureID, this->x, this->y, NULL, "menu");
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
