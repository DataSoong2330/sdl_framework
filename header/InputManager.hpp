#ifndef __INPUTMANAGER__
#define __INPUTMANAGER__

#include <iostream>
#include <SDL2/SDL.h>
//#include "SDL.h"
#include <vector>

#include "../header/Vector2D.hpp"

// enum for the mouse buttons
enum mouse_buttons { LEFT, MIDDLE, RIGHT };

class InputManager
{
    public:
        // singleton instance
        static InputManager* Instance()
        {
            if(inputManager == 0)
                inputManager = new InputManager();

            return inputManager;
        }

        // initializes the joysticks
        void initializeJoysticks();
        inline const bool& joysticksInitialised() const { return this->joysticksInitialized; }

        // reset the mouse buttons
        void reset();

        // check if a single key was triggered
        bool isKeyTriggered(const SDL_Scancode keyCode) /*const*/;
        // check if a single key was pressed
        bool isKeyPressed(const SDL_Scancode keycode) const;
        // check if a single key was released
        bool isKeyReleased(const SDL_Scancode keyCode) const;

        // joystick events
        int getAxisX(int joy, int stick) const;
        int getAxisY(int joy, int stick) const;
        inline const bool getButtonState(int joy, int buttonNumber) const { return this->buttonStates[joy][buttonNumber]; }

        // check if a certain mouse button was used
        inline const bool getMouseButtonState(mouse_buttons mouseButton) const { return this->mouseButtonStates[mouseButton]; }
        // get the position of the mouse cursor
        inline Vector2D* getMousePosition() const { return this->mousePosition; }

        // update the input
        void update();

    protected:
    private:

        // private ctor and dtor for singleton
        InputManager();
        ~InputManager();

        // update the pressed or released keys
        void onKeyDown(SDL_Event &event);
        void onKeyUp(SDL_Event &event);

        // mouse moved?
        void onMouseMove(SDL_Event& event);
        // if a mouse button was pressed or released
        void onMouseButtonDown(SDL_Event &event);
        void onMouseButtonUp(SDL_Event &event);

        // was the joystick moved?
        void onJoystickAxisMove(SDL_Event &event);
        // was a joystick button pressed or released
        void onJoystickButtonDown(SDL_Event &event);
        void onJoystickButtonUp(SDL_Event &event);

        //Uint8 previousInput[SDL_NUM_SCANCODES];
        //Uint8 currentInput[SDL_NUM_SCANCODES];
        const Uint8* currentKeystates;
        bool aIsTriggered[SDL_NUM_SCANCODES];

        // joystick variables
        std::vector<std::pair<Vector2D*, Vector2D*>> joystickValues;
        std::vector<SDL_Joystick*> joysticks;
        std::vector<std::vector<bool>> buttonStates;
        bool joysticksInitialized;
        static const int joystickDeadZone = 10000;

        // how many mouse buttons and an array for which was pressed
        std::vector<bool> mouseButtonStates;
        // the current mouse position
        Vector2D* mousePosition;

        // singleton instance
        static InputManager* inputManager;
};

#endif // __INPUTMANAGER__
