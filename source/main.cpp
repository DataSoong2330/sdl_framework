#include "../header/Game.hpp"

int main(int argc, char* argv[])
{
    // init the game
    if(Game::Instance()->init())
    {
        // while not quitted
        while(Game::Instance()->isRunning())
        {
            // handle the events
            Game::Instance()->handleEvents();
            // update events
            Game::Instance()->update();
            // render everything
            Game::Instance()->render();

            // wait for next frame if necessary
            Game::Instance()->delay();
        }

        Game::Instance()->freeMemory();
    }
}
