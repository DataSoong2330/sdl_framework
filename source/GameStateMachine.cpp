#include "../header/GameStateMachine.hpp"

// pushes a state in the game state machine
void GameStateMachine::pushState(GameState* state)
{
    // push it on the backe
    this->gameStates.push_back(state);
    // enter the state
    this->gameStates.back()->onEnter();
}

void GameStateMachine::popState()
{
    // if there is a state to remove
    if(!this->gameStates.empty())
    {
        // leave the state...
        this->gameStates.back()->onExit();
        // ... and remove it
        this->gameStates.pop_back();
    }
}

// change the state in another
void GameStateMachine::changeState(GameState* state)
{
    // if there is a state already
    if(!this->gameStates.empty())
    {
        // if the state to change in is in the vector already...
        if(this->gameStates.back()->getStateID() == state->getStateID())
            return; // ... do nothing

        // if not, leave the state
        this->gameStates.back()->onExit();
        this->gameStates.pop_back();
    }

    // enter the new state
    state->onEnter();

    // push the state into the vector
    this->gameStates.push_back(state);
}

// update the current game state if there is one
void GameStateMachine::update()
{
    if(!this->gameStates.empty())
        this->gameStates.back()->update();
}

// renders the current game state if there is one
void GameStateMachine::render()
{
    if(!this->gameStates.empty())
        this->gameStates.back()->render();
}
