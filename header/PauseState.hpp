#ifndef __PAUSESTATE__
#define __PAUSESTATE__

#include "../header/GameState.hpp"

class PauseState : public GameState
{
    public:
        PauseState(GameStateMachine& stateMachine);

        // update input and so on
        bool update();
        // render all necessary stuff of a certain state
        void render();
        // handle Events
        bool handleEvents();

        // loads all images, sounds and stuff
        bool onEnter(std::string fileName);
        // frees the memory allocated on entering a state
        bool onExit();

    protected:
    private:
};

#endif // __PAUSESTATE__
