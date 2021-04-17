#include "../header/Game.hpp"
#include "../header/InputManager.hpp"
#include "../header/Logfile.hpp"
#include "../header/MenuState.hpp"
#include "../header/SoundManager.hpp"
#include "../header/TextureManager.hpp"

Game* Game::game = 0;

// initializes the variables
Game::Game()
{
    Logfile::Instance()->createLogfile("SDL Framework Development.html");
    Logfile::Instance()->WriteTopic("Logging Errors and Successes", 8);

    this->running = false;

    this->window = NULL;
    this->renderer = NULL;

    this->countedFrames = 0;
    this->avgFPS = 0.0f;
    this->frameTimeIndex = 0;
    this->frameTimeLast = SDL_GetTicks();
    this->getTicks = 0;

    this->frameTimes[arraySize] = {0};
}

// cleans up everything
Game::~Game()
{
    TextureManager::Instance()->freeMemory();
    SoundManager::Instance()->freeMemory();

    SDL_DestroyRenderer(this->renderer);
    this->renderer = NULL;
    SDL_DestroyWindow(this->window);
    this->window = NULL;

    SDL_Quit();

    Logfile::Instance()->quitLogging();
}

// inits the game and the necessary variables
bool Game::init()
{
    // saves the success
    bool success = false;

    // init the sdl with SDL_INIT_EVERYTHING
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        Logfile::Instance()->Textout(FONTCOLORS::GREEN, "SDL successfully initialised\n");

        // create the sdl window
        // parameters: name, xpos, ypos, width, height, shown
        this->window = SDL_CreateWindow("Framework Development", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);

        // check the window creation
        if(this->window != 0)
        {
            Logfile::Instance()->Textout(FONTCOLORS::GREEN, "Window created succesfully\n");

            // create the renderer
            // parameters: the window, index of rendering driver (-1 for first available), hardware accelerated
            this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);

            // check the renderer creation
            if(this->renderer != 0)
            {
                Logfile::Instance()->Textout(FONTCOLORS::GREEN, "Renderer created successfully\n");

                // set the draw color to black (r = 0, g = 0, b = 0, a = 255)
                SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);

                // set the flags for the texture manager init
                int imgFlags = IMG_INIT_JPG;

                // check if the TextureManager can be initiliazed
                if(TextureManager::Instance()->initTextureManager(imgFlags))
                {
                    int sfxFlags = MIX_INIT_MP3;

                    if(SoundManager::Instance()->init(sfxFlags, 44100, AUDIO_S16, 2, 4096))
                    {
                        this->gameStateMachine = new GameStateMachine();
                        this->gameStateMachine->pushState(new MenuState());

                        success = true;
                        this->running = true;
                    }
                    else
                    {
                        Logfile::Instance()->Textout(FONTCOLORS::RED, "Failed to init Sound Manager\n");
                        Logfile::Instance()->Textout(Mix_GetError());
                    }
                }
                else
                {
                    // some error in the TextureManager
                    Logfile::Instance()->Textout(FONTCOLORS::RED, "Failed to init Texture Manager\n");
                }
            }
            else
            {
                Logfile::Instance()->Textout(FONTCOLORS::RED, "Renderer could not be created\n");
                Logfile::Instance()->Textout(SDL_GetError());
            }
        }
        else
        {
            // error handling for the window
            Logfile::Instance()->Textout(FONTCOLORS::RED, "Window could not be created\n");
            Logfile::Instance()->Textout(SDL_GetError());
        }
    }
    else
    {
        // error handling for the SDL
        Logfile::Instance()->Textout(FONTCOLORS::RED, "SDL could not be initialized\n");
        Logfile::Instance()->Textout(SDL_GetError());
    }

    return success;
}

// delay waits a specific time after a rendering a frame
void Game::delay()
{
    // determines which element of the array will be changed
    this->frameTimeIndex = this->countedFrames % arraySize;

    /*
    // get the current time
    this->getTicks = SDL_GetTicks();

    // saves the time the current frame needed into the array
    this->frameTimes[this->frameTimeIndex] = this->getTicks - this->frameTimeLast;

    // saves the current time for next delay
    this->frameTimeLast = this->getTicks;

    // delete old value
    this->avgFPS = 0.f;

    // add up all frame times in the array
    for(unsigned i = 0; i < arraySize; i++)
        this->avgFPS += this->frameTimes[i];

    // get the average frame time
    this->avgFPS /= arraySize;

    // calculate the average frames per second
    this->avgFPS = 1000.f / avgFPS;

    // add the avgFPS to the texture manager
    TextureManager::Instance()->loadTextTexture("oxyReg30", (std::to_string(avgFPS) + " FPS"), "white", "Fps");*/

    // if the frame was ready to early it will wait for the rest of the time
    if(this->frameTimes[this->frameTimeIndex] < DELAY_TIME)
        SDL_Delay(DELAY_TIME - this->frameTimes[this->frameTimeIndex]);
}

// renders all necessary textures
void Game::render()
{
    // reset the draw color
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);

    // clear the renderer
    SDL_RenderClear(this->renderer);

    // renders the current game state
    this->gameStateMachine->render();

    // render all present textures
    SDL_RenderPresent(this->renderer);

    // one more frame which was rendered
    ++this->countedFrames;
}

// check for events and respond to them
void Game::handleEvents()
{
}

// update the game
void Game::update()
{
    // update the input
    InputManager::Instance()->update();
    // update the current state
    this->gameStateMachine->update();
}
