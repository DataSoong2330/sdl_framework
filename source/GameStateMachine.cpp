#include <cassert>

#include "../header/GameStateMachine.hpp"

GameStateMachine::GameStateMachine() : gameStates(), pendingList(), factories()
{
}

// pushes a state in the game state machine
void GameStateMachine::pushState(States stateID)
{
    // pushs a push request to the list
    this->pendingList.push_back(PendingChange(StateAction::Push, stateID));
}

void GameStateMachine::popState()
{
    // pushs a pop request to the list
    this->pendingList.push_back(PendingChange(StateAction::Pop));
}

void GameStateMachine::clearStates()
{
    this->pendingList.push_back(PendingChange(StateAction::Clear));
}

// update the current game state if there is one
void GameStateMachine::update()
{
    for(auto itr = this->gameStates.rbegin(); itr != this->gameStates.rend(); ++itr)
    {
        if(!(*itr)->update())
            break;
    }

    this->applyPendingChanges();
}

// renders the current game state if there is one
void GameStateMachine::render()
{
    for(auto &state : this->gameStates)
        state->render();
}

void GameStateMachine::handleEvents()
{
    for(auto itr = this->gameStates.rbegin(); itr != this->gameStates.rend(); ++itr)
    {
        if(!(*itr)->handleEvents())
            break;
    }

    this->applyPendingChanges();
}

std::unique_ptr<GameState> GameStateMachine::createState(States stateID)
{
	auto found = factories.find(stateID);
	assert(found != factories.end());

	return found->second();
}

void GameStateMachine::applyPendingChanges()
{
    for(auto item : this->pendingList)
	{
		switch (item.action)
		{
			case StateAction::Push:     this->gameStates.push_back(createState(item.stateID));

                                        switch (item.stateID)
                                        {
                                            case States::Menu:      this->gameStates.back()->onEnter("states/menuState.json");
                                                                    break;
                                            case States::Play:      this->gameStates.back()->onEnter("states/playState.json");
                                                                    break;
                                            case States::Pause:     this->gameStates.back()->onEnter("states/pauseState.json");
                                                                    break;
                                            case States::GameOver:  this->gameStates.back()->onEnter("states/gameOverState.json");
                                                                    break;
                                            case States::None:      break;
                                        }

                                        break;

			case StateAction::Pop:      this->gameStates.back()->onExit();
                                        this->gameStates.pop_back();
                                        break;

			case StateAction::Clear:    for(auto itr = this->gameStates.rbegin(); itr != this->gameStates.rend(); ++itr)
                                        {
                                            if(!(*itr)->onExit())
                                                break;
                                        }
                                        this->gameStates.clear();
                                        break;
		}
	}

	pendingList.clear();
}

GameStateMachine::PendingChange::PendingChange(StateAction action, States stateID) : action(action), stateID(stateID)
{
}
