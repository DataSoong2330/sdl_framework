#ifndef __GAMEOVERSTATE__
#define __GAMEOVERSTATE__

#include "../header/GameState.hpp"

class GameOverState : public GameState
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
        inline const std::string getStateID() const { return gameOverID; }

    protected:
    private:
        static const std::string gameOverID;

        static void gameOverToMain();
        static void restartPlay();
};

#endif // __GAMEOVERSTATE__
