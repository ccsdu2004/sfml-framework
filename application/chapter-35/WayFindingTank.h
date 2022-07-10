#pragma once
#include <MovingSprite.h>
#include <StateMachine.h>
#include "SpriteIdleState.h"
#include "SpriteForwardState.h"
#include "SpriteRotateState.h"
#include "SpriteErrorState.h"

class WayFindingTank : public MovingSprite
{
public:
    WayFindingTank() = delete;
    WayFindingTank(const std::string &image, float x, float y);
public:
    void init(TileMapPointer inputTileMap);

    void makeForward();
    void makeTurnLeft();
    void makeTurnRight();

    std::shared_ptr<State> getCurrentState()const;
private:
    bool shouldForward();
    void onUpdateMovingSprite(float deltaTime)override;
private:
    std::weak_ptr<TileMap> tileMap;

    std::shared_ptr<StateMachine> tankStateMachine;

    std::shared_ptr<State> errorState;
    std::shared_ptr<State> idleState;

    std::shared_ptr<State> forwardState;
    std::shared_ptr<State> turnLeftState;
    std::shared_ptr<State> turnRightState;
};
