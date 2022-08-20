#ifndef __GAMESTATE__
#define __GAMESTATE__

#include <fstream>
#include <functional>
#include <map>
#include <nlohmann/json.hpp>
#include <SDL2/SDL.h>
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

        // sdl rectangle for saving the current screen origin and resolution
        // necessary for aligning buttons and reset to main viewport
        SDL_Rect mainViewport;

        nlohmann::json stateJson;

        std::map<std::string, std::function<void()>> functionMap;

    private:
};

#endif // __GAMESTATE__
