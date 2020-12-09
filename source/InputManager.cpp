#include "../header/Game.hpp"
#include "../header/InputManager.hpp"

// initialize the static singleton variable
InputManager* InputManager::inputManager = 0;

// initializes the member variables
InputManager::InputManager() :  currentKeystates(NULL),
                                joysticksInitialized(false),
                                mousePosition(new Vector2D(0.0f, 0.0f))

{
    for(int i = 0; i < 3; i++)
        this->mouseButtonStates.push_back(false);

    memset(this->aIsTriggered, false, sizeof(bool) * SDL_NUM_SCANCODES);
}

// frees the memory
InputManager::~InputManager()
{
    delete this->mousePosition;
    delete this->currentKeystates;
    //delete this->keysRepeated;

    this->joystickValues.clear();

    if(joysticksInitialized)
    {
        for(int i = 0; i < SDL_NumJoysticks(); i++)
            SDL_JoystickClose(this->joysticks[i]);
    }

    this->joysticks.clear();
    this->buttonStates.clear();
}

// initializes the joysticks
void InputManager::initializeJoysticks()
{
    // if joystick subsystem wasn't initialized already
    if(SDL_WasInit(SDL_INIT_JOYSTICK) == 0)
        SDL_InitSubSystem(SDL_INIT_JOYSTICK);

    // are the any joysticks available?
    if(SDL_NumJoysticks() > 0)
    {
        // loop through each joystick
        for(int i = 0; i < SDL_NumJoysticks(); i++)
        {
            // create a new joystick
            SDL_Joystick* joy = SDL_JoystickOpen(i);

            // if the joystick was opened correctly
            if(SDL_JoystickOpen(i))
            {
                // save the joystick to the vector
                this->joysticks.push_back(joy);

                // create a pair of values for the axes
                this->joystickValues.push_back(std::make_pair(new Vector2D(0, 0), new Vector2D(0, 0)));

                // create a temp vector for the buttons
                std::vector<bool> tempButtons;

                // init the vector with the standard value (false)
                for(int j = 0; j < SDL_JoystickNumButtons(joy); j++)
                    tempButtons.push_back(false);

                // save the temp vector
                this->buttonStates.push_back(tempButtons);
            }
            else
            {
                // if an error occurred, print it out
                std::cout << "Could not initialize the joystick" << i << "\n";
                std::cout << SDL_GetError() << "\n";
            }
        }

        // enable joystick events
        SDL_JoystickEventState(SDL_ENABLE);
        this->joysticksInitialized = true;

        std::cout << "Initialized " << this->joysticks.size() << " joysticks\n";
    }
    else
    {
        std::cout << "No joysticks initialized" << "\n";
        this->joysticksInitialized = false;
    }
}

// resets the status of the mouse buttons
void InputManager::reset()
{
    this->mouseButtonStates[mouse_buttons::LEFT] = false;
    this->mouseButtonStates[mouse_buttons::MIDDLE] = false;
    this->mouseButtonStates[mouse_buttons::RIGHT] = false;
}

// check if a single key was triggered
bool InputManager::isKeyTriggered(const SDL_Scancode keyCode) //const
{
    if(this->aIsTriggered[keyCode] == true)
    {
        this->aIsTriggered[keyCode] = false;
        return true;
    }
    else
    {
        return false;
    }
}

// check if a single key was pressed
bool InputManager::isKeyPressed(const SDL_Scancode keyCode) const
{
    //return (this->aIsTriggered[keyCode]);
    if(currentKeystates != NULL)
        return (currentKeystates[keyCode]);
    else
        return false;
}

// check if a single key was released
bool InputManager::isKeyReleased(const SDL_Scancode keyCode) const
{
    //return (this->aIsTriggered[keyCode]);
    if(currentKeystates != NULL)
        return (currentKeystates[keyCode]);
    else
        return false;
}

int InputManager::getAxisX(int joy, int stick) const
{
    // init return value
    int xAxis = 0;

    // if there are joysticks
    if(this->joysticks.size() > 0)
    {
        if(stick == 1)
            xAxis = this->joystickValues[joy].first->getXPos();
        else if(stick == 2)
            xAxis = this->joystickValues[joy].second->getXPos();
    }

    // return value
    return xAxis;
}

int InputManager::getAxisY(int joy, int stick) const
{
    // init return value
    int yAxis = 0;

    // if there are joysticks
    if(this->joysticks.size() > 0)
    {
        if(stick == 1)
            yAxis = this->joystickValues[joy].first->getYPos();
        else if(stick == 2)
            yAxis = this->joystickValues[joy].second->getYPos();
    }

    // return value
    return yAxis;
}

// updates the variables
void InputManager::update()
{
    SDL_Event event;

    SDL_WaitEvent(&event);

    do
    {
        switch(event.type)
        {
            case SDL_QUIT:              Game::Instance()->quit();
                                        break;
            case SDL_JOYAXISMOTION:     this->onJoystickAxisMove(event);
                                        break;
            case SDL_JOYBUTTONDOWN:     this->onJoystickButtonDown(event);
                                        break;
            case SDL_JOYBUTTONUP:       this->onJoystickButtonUp(event);
                                        break;
            case SDL_MOUSEMOTION:       this->onMouseMove(event);
                                        break;
            case SDL_MOUSEBUTTONDOWN:   this->onMouseButtonDown(event);
                                        break;
            case SDL_MOUSEBUTTONUP:     this->onMouseButtonUp(event);
                                        break;
            case SDL_KEYDOWN:           this->onKeyDown(event);
                                        break;
            case SDL_KEYUP:             this->onKeyUp(event);
                                        break;
            default:                    break;
        }
    } while (SDL_PollEvent(&event));
}

// updates the mouse coordinates
void InputManager::onMouseMove(SDL_Event &event)
{
    this->mousePosition->setXPos(event.motion.x);
    this->mousePosition->setYPos(event.motion.y);
}

// updates the mouse buttons if a mouse button was pressed
void InputManager::onMouseButtonDown(SDL_Event &event)
{
    if(event.button.button == SDL_BUTTON_LEFT)
        mouseButtonStates[mouse_buttons::LEFT] = true;
    if(event.button.button == SDL_BUTTON_MIDDLE)
        mouseButtonStates[mouse_buttons::MIDDLE] = true;
    if(event.button.button == SDL_BUTTON_RIGHT)
        mouseButtonStates[mouse_buttons::RIGHT] = true;
}

// updates the mouse buttons if a mouse button was released
void InputManager::onMouseButtonUp(SDL_Event &event)
{
    if(event.button.button == SDL_BUTTON_LEFT)
        mouseButtonStates[mouse_buttons::LEFT] = false;
    if(event.button.button == SDL_BUTTON_MIDDLE)
        mouseButtonStates[mouse_buttons::MIDDLE] = false;
    if(event.button.button == SDL_BUTTON_RIGHT)
        mouseButtonStates[mouse_buttons::RIGHT] = false;
}

// update the keyboard on key press
void InputManager::onKeyDown(SDL_Event &event)
{
    if(event.key.repeat == 0)
    {
        this->aIsTriggered[event.key.keysym.scancode] = true;
    }
    this->currentKeystates = SDL_GetKeyboardState(NULL);
}

// update the keyboard on key release
void InputManager::onKeyUp(SDL_Event &event)
{
    this->aIsTriggered[event.key.keysym.scancode] = false;
    this->currentKeystates = SDL_GetKeyboardState(NULL);
}

// check for movement on the joystick axis
void InputManager::onJoystickAxisMove(SDL_Event &event)
{
    int whichOne = event.jaxis.which;

    // left stick move left or right
    if(event.jaxis.axis == 0)
    {
        if(event.jaxis.value > joystickDeadZone)
            this->joystickValues[whichOne].first->setXPos(1);
        else if(event.jaxis.value < -joystickDeadZone)
            this->joystickValues[whichOne].first->setXPos(-1);
        else
            this->joystickValues[whichOne].first->setXPos(0);
    }

    // left stick move up or down
    if(event.jaxis.axis == 1)
    {
        if(event.jaxis.value > joystickDeadZone)
            this->joystickValues[whichOne].first->setYPos(1);
        else if(event.jaxis.value < -joystickDeadZone)
            this->joystickValues[whichOne].first->setYPos(-1);
        else
            this->joystickValues[whichOne].first->setYPos(0);
    }

    // right stick move left or right
    if(event.jaxis.axis == 3)
    {
        if(event.jaxis.value > joystickDeadZone)
            this->joystickValues[whichOne].second->setXPos(1);
        else if(event.jaxis.value < -joystickDeadZone)
            this->joystickValues[whichOne].second->setXPos(-1);
        else
            this->joystickValues[whichOne].second->setXPos(0);
    }

    // right stick move up or down
    if(event.jaxis.axis == 1)
    {
        if(event.jaxis.value > joystickDeadZone)
            this->joystickValues[whichOne].second->setYPos(1);
        else if(event.jaxis.value < -joystickDeadZone)
            this->joystickValues[whichOne].second->setYPos(-1);
        else
            this->joystickValues[whichOne].second->setYPos(0);
    }
}

// sets the joystick button to true on press
void InputManager::onJoystickButtonDown(SDL_Event &event)
{
    // which joystick was used?
    int whichOne = event.jaxis.which;

    // set the button of the joystick to true
    this->buttonStates[whichOne][event.jbutton.button] = true;
}

// sets the joystick button to false on release
void InputManager::onJoystickButtonUp(SDL_Event &event)
{
    // which joystick was used?
    int whichOne = event.jaxis.which;

    this->buttonStates[whichOne][event.jbutton.button] = false;
}
