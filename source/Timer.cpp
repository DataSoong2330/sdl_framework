#include "../header/Timer.hpp"

// ctor for standard init
Timer::Timer()
{
    this->pausedTicks = 0;
    this->startTicks = 0;

    this->paused = false;
    this->started = false;
}

// starts the timer
void Timer::start()
{
    this->started = true;
    this->paused = false;

    // get the amount of milliseconds the SDL is running already
    this->startTicks = SDL_GetTicks();
    this->pausedTicks = 0;
}

// stops the timer = everything to false and zero
void Timer::stop()
{
    this->started = false;
    this->paused = false;

    this->startTicks = 0;
    this->pausedTicks = 0;
}

// pauses the timer only if it's started and not paused
void Timer::pause()
{
    if(this->started && !this->paused)
    {
        this->paused = true;
        // saves the time from start to the current value
        this->pausedTicks = SDL_GetTicks() - this->startTicks;
        this->startTicks = 0;
    }
}

// gets the timer running again
void Timer::unpause()
{
    if(this->started && this->paused)
    {
        this->paused = false;
        // the current value minus the time the timer was running
        // before it was paused
        // example: the timer was running 5.000 ms before the pause
        // this sets it 5.000 ms before the current value
        this->startTicks = SDL_GetTicks() - this->pausedTicks;
        this->pausedTicks = 0;
    }
}

// gets the milliseconds the timer was running already
Uint32 Timer::getTicks()
{
    // the time init with zero
    Uint32 time = 0;

    if(this->started)
    {
        if(this->paused)
            // the time measured before the pause
            time = this->pausedTicks;
        else
            // the time measured from start to current value
            time = SDL_GetTicks() - this->startTicks;
    }

    return time;
}
