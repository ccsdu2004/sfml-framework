#pragma once
#include <cmath>
#include <optional>
#include <Animation.h>
#include <MovingSprite.h>
#include <StateMachine.h>
#include <Util.h>

#define ROTATE_SPEED 36.0f

class SpriteTurnLeftState : public State
{
public:
    SpriteTurnLeftState(ObjectPointer inputSprite);
public:
    void onEnter() override;
    void onLeave() override;
    void update(float deltaTime) override;
private:
    float startAngle = 0.0f;
};

class SpriteTurnRightState : public State
{
public:
    SpriteTurnRightState(ObjectPointer inputSprite);
public:
    void onEnter() override;
    void onLeave() override;
    void update(float deltaTime) override;
private:
    float startAngle = 0.0f;
};

enum RotateDirection {
    RotateDirection_Left,
    RotateDirection_Right
};
