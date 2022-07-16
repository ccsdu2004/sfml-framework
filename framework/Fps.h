#pragma once
#include <Component.h>

class Fps : public Component
{
public:
    Fps();
    ~Fps();
public:
    float getFps()const;
    void update(float deltaTime) override;
private:
    std::unique_ptr<class FpsData> data;
};

