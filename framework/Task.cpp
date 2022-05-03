#include <Task.h>

class TaskData
{
public:
    bool started = false;
    bool aborted = false;
};

Task::Task():
    data(new TaskData())
{
}

Task::~Task()
{
}

bool Task::isRunning()const
{
    return data->started;
}

void Task::start()
{
    if(data->started)
        return;
    data->started = true;
}

void Task::abort()
{
    data->aborted = true;
}

void Task::update(float deltaTime)
{

}
