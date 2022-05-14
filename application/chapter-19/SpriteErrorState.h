#pragma once
#include <StateMachine.h>

class SpriteErrorState final : public State
{
public:
    SpriteErrorState();
public:
    void update(float deltaTime) override;
private:
    void onEnter() override;
    void onLeave() override;
};
