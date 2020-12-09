#ifndef __GAMESTATE__
#define __GAMESTATE__

#include <string>
#include <vector>

#include "../header/MenuButton.hpp"

class GameState
{
    public:
        // update input and so on
        virtual void update() = 0;
        // render all necessary stuff of a certain state
        virtual void render() = 0;

        // loads all images, sounds and stuff
        virtual bool onEnter() = 0;
        // frees the memory allocated on entering a state
        virtual bool onExit() = 0;

        // gets the state ID like Menu, Play, GameOver and so on
        virtual const std::string getStateID() const = 0;

    protected:
        std::vector<GameObject*> gameObjects;
        //ButtonCoordinate coordinate;

        // necessary variables for aligning the buttons
        int x, y, width, height;

    private:
};

#endif // __GAMESTATE__
