#pragma once
#include <StateMachine.h>

class SpriteIdleState : public State
{
public:
    SpriteIdleState();
public:
    void onEnter() override;
    void onLeave() override;
    void update(float deltaTime) override;
private:
};
