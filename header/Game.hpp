#ifndef __GAME__
#define __GAME__

#include <iostream>
#include <fstream>
#include <map>
#include <nlohmann/json.hpp>
#include <SDL2/SDL.h>

#include "../header/GameStateMachine.hpp"
#include "../header/Texture.hpp"
#include "../header/Timer.hpp"

class Game
{
    public:
        // access for the static singleton variable
        static Game* Instance()
        {
            if(game == 0)
                game = new Game();

            return game;
        }

        // initializes the game
        bool init();

        // is the game running?
        inline const bool& isRunning() const { return this->running; }
        // access to the renderer
        inline SDL_Renderer* getRenderer() const { return this->renderer; }
        // access to the window
        inline SDL_Window* getWindow() const { return this->window; }

        // if the frame was ready too early there will be a delay
        void delay();

        // renders the current screens, texts and backgrounds
        void render();
        // checks the mouse and keyboard inputs
        void handleEvents();
        // gets the mouse and keyboard inputs
        void update();

        // get the game state machine
        //inline GameStateMachine* getGameStateMachine() { return this->gameStateMachine; }

        // quits the game
        void quit() { this->running = false; }

        // frees the memory used by the game
        void freeMemory() { delete game; game = nullptr; }

    protected:
    private:
        // ctor and dtor as private for singleton
        Game();
        ~Game();

        // static game pointer for singleton
        static Game* game;

        // nlohmann::json object
        nlohmann::json config;

        // window resolution
        static const int width = 1024;
        static const int height = 768;

        // frame per second and the time for a single frame
        static const Uint32 FPS = 60;
        static const Uint32 DELAY_TIME = 1000 / FPS;

        // variables for measuring and calculating the current fps
        Uint32 lastTicks;
        Uint32 currentTicks;
        Uint32 totalTicksElapsed;
        Uint32 countedFrames;
        int avgFPS;

        // register the states for the GameStateMachine
        void registerStates();
        // the game state machine
        std::unique_ptr<GameStateMachine> gameStateMachine;

        // is the game running?
        bool running;

        // the window and renderer
        SDL_Window* window;
        SDL_Renderer* renderer;
};

#endif // __GAME__
