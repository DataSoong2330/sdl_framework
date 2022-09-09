#include "../header/Game.hpp"
#include "../header/GameOverState.hpp"
#include "../header/InputManager.hpp"
#include "../header/Logfile.hpp"
#include "../header/MenuState.hpp"
#include "../header/PauseState.hpp"
#include "../header/PlayState.hpp"
#include "../header/SettingState.hpp"
#include "../header/SoundManager.hpp"
#include "../header/SystemTime.hpp"
#include "../header/TextureManager.hpp"

Game* Game::game = 0;

#include <sstream>

// initializes the variables
Game::Game()
{
    MyTime::Instance()->startTimer();

    std::ifstream configFile("config.json");

    if(configFile)
    {
        this->config = nlohmann::json::parse(configFile);

        std::string fileName = MyTime::Instance()->getDate() + ".json";
        Logfile::Instance()->createLogfile(fileName);

        std::string temp = "";
        nlohmann::json j_string = this->config["application"];
        j_string.get_to(temp);
        Logfile::Instance()->Textout("Application", "Date", MyTime::Instance()->getDate());
        Logfile::Instance()->Textout("Application", "Time", MyTime::Instance()->getTime());
    }
    else
    {
        std::cout << "warning: config.json could not be opened" << "\n";
    }

    this->running = false;

    this->window = NULL;
    this->renderer = NULL;

    this->countedFrames = 0;
    this->avgFPS = 0;
    this->lastTicks = 0;
    this->currentTicks = 0;
    this->totalTicksElapsed = 0;
}

// cleans up everything
Game::~Game()
{
    this->gameStateMachine->clearStates();
    //this->gameStateMachine->update();

    TextureManager::Instance()->freeMemory();
    SoundManager::Instance()->freeMemory();

    SDL_DestroyRenderer(this->renderer);
    this->renderer = NULL;
    SDL_DestroyWindow(this->window);
    this->window = NULL;

    SDL_Quit();

    MyTime::Instance()->endTimer();

    Logfile::Instance()->Textout("Application", "Runtime", MyTime::Instance()->getTimerValue());

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
        Logfile::Instance()->Textout("Game", "SDL_Init", "SDL successfully initialised");

        nlohmann::json j_string = this->config["application"];
        std::string jsonValue = "";
        j_string.get_to(jsonValue);

        // create the sdl window
        // parameters: name, xpos, ypos, width, height, shown
        this->window = SDL_CreateWindow(jsonValue.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        this->config["resolution"]["width"], this->config["resolution"]["height"],
                                        SDL_WINDOW_SHOWN);

        // check the window creation
        if(this->window != 0)
        {
            Logfile::Instance()->Textout("Game", "CreateWindow", "Window created succesfully");

            // create the renderer
            // parameters: the window, index of rendering driver (-1 for first available), hardware accelerated
            this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);

            // check the renderer creation
            if(this->renderer != 0)
            {
                Logfile::Instance()->Textout("Game", "Renderer", "Renderer created successfully");

                // set the draw color to black (r = 0, g = 0, b = 0, a = 255)
                SDL_SetRenderDrawColor(this->renderer,  this->config["renderer"]["red"],
                                                        this->config["renderer"]["green"],
                                                        this->config["renderer"]["blue"],
                                                        this->config["renderer"]["alpha"]);

                // set the flags for the texture manager init
                int imgFlags = IMG_INIT_JPG;

                // check if the TextureManager can be initiliazed
                if(TextureManager::Instance()->initTextureManager(imgFlags))
                {
                    int sfxFlags = MIX_INIT_MP3;

                    if(SoundManager::Instance()->init(sfxFlags, 44100, AUDIO_S16, 2, 4096))
                    {
                        this->gameStateMachine = std::make_unique<GameStateMachine>();
                        this->registerStates();
                        this->gameStateMachine->pushState(States::Menu);

                        success = true;
                        this->running = true;
                    }
                    else
                    {
                        Logfile::Instance()->Textout("Game", "Mixer", Mix_GetError());
                    }
                }
                else
                {
                    // some error in the TextureManager
                    Logfile::Instance()->Textout("Game", "Framework", "Failed to init Texture Manager");
                }
            }
            else
            {
                Logfile::Instance()->Textout("Game", "Renderer", SDL_GetError());
            }
        }
        else
        {
            // error handling for the window
            Logfile::Instance()->Textout("Game", "CreateWindow", SDL_GetError());
        }
    }
    else
    {
        // error handling for the SDL
        Logfile::Instance()->Textout("Game", "SDL_Init", SDL_GetError());
    }

    return success;
}

// delay waits a specific time after a rendering a frame
void Game::delay()
{
    this->currentTicks = SDL_GetTicks();
    Uint32 ticksElapsed = this->currentTicks - this->lastTicks;

    this->totalTicksElapsed += ticksElapsed;

    if(this->totalTicksElapsed > 1000)
    {
        this->avgFPS = this->countedFrames / (this->totalTicksElapsed / 1000);
        this->totalTicksElapsed = 0;
        this->countedFrames = 0;

        // add the avgFPS to the texture manager
        TextureManager::Instance()->loadTextTexture("oxyReg30", (std::to_string(avgFPS) + " FPS"), "white", "Fps");
    }

    // if the frame was ready to early it will wait for the rest of the time
    if(ticksElapsed < DELAY_TIME)
        SDL_Delay(DELAY_TIME - ticksElapsed);

    this->lastTicks = this->currentTicks;
}

// renders all necessary textures
void Game::render()
{
    // reset the draw color
    SDL_SetRenderDrawColor(this->renderer,  this->config["renderer"]["red"],
                                            this->config["renderer"]["green"],
                                            this->config["renderer"]["blue"],
                                            this->config["renderer"]["alpha"]);

    // clear the renderer
    SDL_RenderClear(this->renderer);

    // renders the current game state
    this->gameStateMachine->render();

    TextureManager::Instance()->drawTexture("Fps", 10, 10, nullptr, "screen", 0.0d, nullptr, SDL_FLIP_NONE);

    // render all present textures
    SDL_RenderPresent(this->renderer);

    // one more frame which was rendered
    ++this->countedFrames;
}

// check for events and respond to them
void Game::handleEvents()
{
    this->gameStateMachine->handleEvents();
}

// update the game
void Game::update()
{
    // update the input
    InputManager::Instance()->update();
    // update the current state
    this->gameStateMachine->update();

    TextureManager::Instance()->loadTextTexture("oxyReg30", (std::to_string(avgFPS) + " FPS"), "white", "Fps");
}

void Game::registerStates()
{
    this->gameStateMachine->registerState<MenuState>(States::Menu);
    this->gameStateMachine->registerState<PlayState>(States::Play);
    this->gameStateMachine->registerState<PauseState>(States::Pause);
    this->gameStateMachine->registerState<GameOverState>(States::GameOver);
    this->gameStateMachine->registerState<SettingState>(States::Setting);
}
