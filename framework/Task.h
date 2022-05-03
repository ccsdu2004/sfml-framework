#pragma once
#include <string>
#include <memory>
#include <boost/signals2.hpp>
#include <NameHolder.h>

class Task : public NameHolder
{
public:
    Task();
    virtual ~Task();
public:
    bool isRunning()const;
    bool isFinished()const;

    void start();
    void abort();
    boost::signals2::signal<void(const std::string&)> finished;
public:
    void update(float deltaTime);
private:
    virtual void onUpdate(float deltaTime);
private:
    std::unique_ptr<class TaskData> data;
};
