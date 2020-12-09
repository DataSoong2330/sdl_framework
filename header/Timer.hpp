#ifndef __TIMER__
#define __TIMER__

#include <SDL2/SDL.h>

// timer class
class Timer
{
    public:
        // member init
        Timer();

        // starts the timer
        void start();
        // stops the timer
        void stop();
        // pauses the timer
        void pause();
        // unpauses the the timer after a pause
        void unpause();

        // how long the timer is or was running
        Uint32 getTicks();

        // was the timer started?
        inline const bool& isStarted() const { return this->started; }
        // is the timer paused?
        inline const bool isPaused() const { return (this->started && this->paused); }

    protected:
    private:
        // this will be set to value in milliseconds
        Uint32 startTicks;
        // this value saves the ticks from start to the moment of pausing the timer
        Uint32 pausedTicks;

        // is the timer started?
        bool started;
        // is the timer paused?
        bool paused;
};

#endif // __TIMER
