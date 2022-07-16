#pragma once
#include <cmath>
#include <optional>
#include <MovingSprite.h>
#include <StateMachine.h>
#include <TileMap.h>
#include <Util.h>

#define FORWARD_SPEED 3.0f

class SpriteForwardState : public State
{
public:
    SpriteForwardState(std::shared_ptr<TileMap> inputTileMap, ObjectPointer inputSprite);
public:
    void onEnter() override;
    void onLeave() override;
    void update(float deltaTime) override;
private:
    std::weak_ptr<TileMap> tileMap;
    sf::Vector2f targetPosition;
    float lastDistance = 0;
};
