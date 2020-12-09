#ifndef __GAMESTATEMACHINE__
#define __GAMESTATEMACHINE__

#include "../header/GameState.hpp"

class GameStateMachine
{
    public:
        // insert a state to the vector
        void pushState(GameState* state);
        // change the state
        void changeState(GameState* state);
        // deletes the last state
        void popState();

        // update the current game state
        void update();
        // render the current game state
        void render();

    protected:
    private:
        // a vector of states
        std::vector<GameState*> gameStates;
};

#endif // __GAMESTATEMACHINE__
