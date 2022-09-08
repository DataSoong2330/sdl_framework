#ifndef __GAMESTATEMACHINE__
#define __GAMESTATEMACHINE__

#include <functional>
#include <map>
#include <memory>
#include <utility>

#include "../header/CommonEnums.hpp"
#include "../header/GameState.hpp"

class GameStateMachine
{
    public:
        // an explicit ctor
        explicit GameStateMachine();

        // template for state registration
        template <typename T>
		void registerState(States stateID);

        // insert a state to the vector
        void pushState(States stateID);
        // change the state
        void popState();
        // deletes the last state
        void clearStates();

        // update the current game state
        void update();
        // render the current game state
        void render();
        // handles events in the game state machine
        void handleEvents();

        // is the state stack empty?
		inline bool isEmpty() const { return this->gameStates.empty(); }

    protected:
    private:
        // creates a state from the factories map below
        std::unique_ptr<GameState> createState(States stateID);
        // applies all changes to the state machine
        void applyPendingChanges();

        // a structure for a pending change
        struct PendingChange
		{
			explicit PendingChange(StateAction action, States stateID = States::None);

			StateAction action;
			States stateID;
		};

        // a vector contains the unique_pointers to the states
        std::vector<std::unique_ptr<GameState>> gameStates;
        // a vector filled with all changes to the state machine
        std::vector<PendingChange> pendingList;

        // a map for the lambda expressions which create the states
        std::map<States, std::function<std::unique_ptr<GameState>()>> factories;
};

// template definition
// the lambda expression:
// [this] are parameters available in the lambda expression
// () are the parameters of the function itself (a constructor in this particular case)
// return std::unique_ptr<State>(new T(*this)); is the code of the lambda expression
template <typename T>
void GameStateMachine::registerState(States stateID)
{
	this->factories[stateID] = [this] ()
	{
		return std::unique_ptr<GameState>(new T(*this));
	};
}

#endif // __GAMESTATEMACHINE__
