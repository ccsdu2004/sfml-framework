#include "SpriteRotateState.h"

SpriteTurnLeftState::SpriteTurnLeftState(ObjectPointer inputSprite):
    State(inputSprite)
{
    setName("turnleft");
}

void SpriteTurnLeftState::onEnter()
{
    if (getTarget().expired())
        return;

    auto sprite = std::dynamic_pointer_cast<MovingSprite>(getTarget().lock());
    startAngle = sprite->getRotate();
    sprite->setRotateVelocity(-ROTATE_SPEED);
}

void SpriteTurnLeftState::onLeave()
{
    if (getTarget().expired())
        return;

    auto sprite = std::dynamic_pointer_cast<MovingSprite>(getTarget().lock());
    sprite->setRotateVelocity(0.0f);
    sprite->setRotate(startAngle - 60.0f);
}

void SpriteTurnLeftState::update(float deltaTime)
{
    (void)deltaTime;
    auto sprite = std::dynamic_pointer_cast<MovingSprite>(getTarget().lock());

    float diffAngle = fabsf(startAngle - sprite->getRotate());
    if (isEqual(diffAngle, 60.0f, 1.0f) || isEqual(diffAngle, 300.0f, 1.0f)) {
        setFinished();
    }
}

SpriteTurnRightState::SpriteTurnRightState(ObjectPointer inputSprite):
    State(inputSprite)
{
    setName("turnright");
}

void SpriteTurnRightState::onEnter()
{
    if (getTarget().expired())
        return;

    auto sprite = std::dynamic_pointer_cast<MovingSprite>(getTarget().lock());
    startAngle = sprite->getRotate();
    sprite->setRotateVelocity(ROTATE_SPEED);
}

void SpriteTurnRightState::onLeave()
{
    if (getTarget().expired())
        return;

    auto sprite = std::dynamic_pointer_cast<MovingSprite>(getTarget().lock());
    sprite->setRotateVelocity(0.0f);
    sprite->setRotate(startAngle + 60.0f);
}

void SpriteTurnRightState::update(float deltaTime)
{
    (void)deltaTime;
    auto sprite = std::dynamic_pointer_cast<MovingSprite>(getTarget().lock());

    float diffAngle = fabsf(sprite->getRotate() - startAngle);
    if (isEqual(diffAngle, 60.0f, 1.0f) || isEqual(diffAngle, 300.0f, 1.0f)) {
        setFinished();
    }
}
