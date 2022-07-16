#include <Fps.h>

class FpsData
{
public:
    float fps = 0.0f;
};

Fps::Fps():
    data(new FpsData)
{
}

Fps::~Fps()
{

}

float Fps::getFps() const
{
    return data->fps;
}

void Fps::update(float deltaTime)
{
    data->fps = 1.0f / deltaTime;
}
