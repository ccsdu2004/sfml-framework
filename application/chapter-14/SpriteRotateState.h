#pragma once
#include <cmath>
#include <optional>
#include <Animation.h>
#include <MovingSprite.h>
#include <StateMachine.h>
#include <Util.h>
#include <iostream>

#define ROTATE_SPEED 18.0f

class SpriteTurnLeftState : public State
{
public:
    SpriteTurnLeftState(ObjectPointer inputSprite):
        State(inputSprite)
    {
    }
public:
    void onEnter() override
    {
        if(getTarget().expired())
            return;

        auto sprite = std::dynamic_pointer_cast<MovingSprite>(getTarget().lock());
        startAngle = sprite->getRotate();
        sprite->setRotateVelocity(-ROTATE_SPEED);
    }

    void onLeave() override
    {
        if(getTarget().expired())
            return;

        auto sprite = std::dynamic_pointer_cast<MovingSprite>(getTarget().lock());
        sprite->setRotateVelocity(0.0f);
    }

    void update(float deltaTime) override
    {
        (void)deltaTime;
        auto sprite = std::dynamic_pointer_cast<MovingSprite>(getTarget().lock());

        float diffAngle = fabsf(startAngle - sprite->getRotate());
        if(isEqual(diffAngle, 60.0f, 1.0f) || isEqual(diffAngle, 300.0f, 1.0f)) {
            setFinished();
        }
    }
private:
    float startAngle = 0.0f;
};

class SpriteTurnRightState : public State
{
public:
    SpriteTurnRightState(ObjectPointer inputSprite):
        State(inputSprite)
    {
    }
public:
    void onEnter() override
    {
        if(getTarget().expired())
            return;

        auto sprite = std::dynamic_pointer_cast<MovingSprite>(getTarget().lock());
        startAngle = sprite->getRotate();
        sprite->setRotateVelocity(ROTATE_SPEED);
    }

    void onLeave() override
    {
        if(getTarget().expired())
            return;

        auto sprite = std::dynamic_pointer_cast<MovingSprite>(getTarget().lock());
        sprite->setRotateVelocity(0.0f);
    }

    void update(float deltaTime) override
    {
        (void)deltaTime;
        auto sprite = std::dynamic_pointer_cast<MovingSprite>(getTarget().lock());

        float diffAngle = fabsf(sprite->getRotate() - startAngle);
        if(isEqual(diffAngle, 60.0f, 1.0f) || isEqual(diffAngle, 300.0f, 1.0f)) {
            setFinished();
        }
    }
private:
    float startAngle = 0.0f;
};
