#ifndef __PLAYSTATE__
#define __PLAYSTATE__

#include "../header/GameState.hpp"

class PlayState : public GameState
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
        inline const std::string getStateID() const { return playID; }

    protected:
    private:
        // state ID
        static const std::string playID;

        static void playToPause();
        static void playToGameOver();
        static void returnToMenu();
};

#endif // __PLAYSTATE__
