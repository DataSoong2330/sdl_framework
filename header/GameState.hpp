#ifndef __GAMESTATE__
#define __GAMESTATE__

#include <fstream>
#include <functional>
#include <map>
#include <nlohmann/json.hpp>
#include <SDL2/SDL.h>
#include <string>
#include <vector>

#include "../header/Asset.hpp"
#include "../header/MenuButton.hpp"
#include "../header/Viewport.hpp"

class GameState
{
    public:
        // update input and so on
        virtual void update();
        // render all necessary stuff of a certain state
        virtual void render();

        // loads all images, sounds and stuff
        virtual bool onEnter(std::string fileName);
        // frees the memory allocated on entering a state
        virtual bool onExit();

        // gets the state ID like Menu, Play, GameOver and so on
        virtual const std::string getStateID() const = 0;

    protected:
        // all game objects like buttons and such
        std::vector<GameObject*> gameObjects;
        //ButtonCoordinate coordinate;

        // a string vector for all resources which were added on entering the state
        std::vector<ImageAsset> assets;
        std::vector<std::string> buttonKeys;
        std::vector<std::string> colorKeys;
        std::vector<std::string> musicKeys;
        std::vector<std::string> soundKeys;
        std::vector<TextAsset> textAssets;
        std::vector<std::string> ttfKeys;
        std::vector<Viewport> viewports;

        // sdl rectangle for saving the current screen origin and resolution
        // necessary for aligning buttons and reset to main viewport
        SDL_Rect screenSize;

        // the state will be parsed into this json object
        nlohmann::ordered_json stateJson;
        // some necessary json objects for loading
        nlohmann::ordered_json name, text, color, ttf;

        // maps the function callbacks by a key
        std::map<std::string, std::function<void()>> functionMap;

    private:
        void loadMusic();
        void loadSound();
        void loadColor();
        void loadTTF();
        void loadViewports();
        void loadTextTextures();
        void loadButtons();
        void loadAssets();
};

#endif // __GAMESTATE__
