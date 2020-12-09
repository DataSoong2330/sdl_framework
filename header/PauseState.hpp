#ifndef __PAUSESTATE__
#define __PAUSESTATE__

#include "../header/GameState.hpp"

class PauseState : public GameState
{
    public:
        // update input and so on
        void update();
        // render all necessary stuff of a certain state
        void render();

        // loads all images, sounds and stuff
        bool onEnter();
        // frees the memory allocated on entering a state
        bool onExit();

        // gets the state ID like Menu, Play, GameOver and so on
        inline const std::string getStateID() const { return pauseID;}

    protected:
    private:
        static const std::string pauseID;

        static void pauseToMain();
        static void resumePlay();
};

#endif // __PAUSESTATE__
