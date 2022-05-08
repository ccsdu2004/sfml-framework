#include "StopWatch.h"
#include <iostream>

class StopWatchData
{
public:
    sf::Clock clock;
    sf::Time time;
    bool running = false;
};

StopWatch::StopWatch():
    data(new StopWatchData)
{
    reset();
}

StopWatch::~StopWatch()
{
}

sf::Time StopWatch::getElapsedTime() const
{
    if (data->running)
        return data->time + data->clock.getElapsedTime();
    else
        return data->time;
}

bool StopWatch::isRunning() const
{
    return data->running;
}

void StopWatch::start()
{
    if (!data->running) {
        data->running = true;
        data->clock.restart();
    }
}

void StopWatch::pause()
{
    if (data->running) {
        data->running = false;
        data->time += data->clock.getElapsedTime();
    }
}

void StopWatch::reset()
{
    data->time = sf::Time::Zero;
    data->running = false;
}

void StopWatch::restart()
{
    reset();
    start();
}
