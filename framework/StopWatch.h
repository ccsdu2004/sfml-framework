#pragma once
#include <memory>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

class StopWatch
{
public:
    StopWatch();
    ~StopWatch();
public:
    sf::Time getElapsedTime() const;
    bool isRunning() const;

    void start();
    void pause();
    void reset();
    void restart();
private:
    std::unique_ptr<class StopWatchData> data;
};
