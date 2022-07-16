#pragma once
#include <StateMachine.h>
#include <TileMap.h>
#include "WayFindingTank.h"

class WayFindingTank;
class SpriteIdleState : public State
{
public:
    SpriteIdleState() = delete;
    SpriteIdleState(std::shared_ptr<TileMap> inputTileMap, ObjectPointer inputTank);
public:
    void onEnter() override;
    void onLeave() override;
    void update(float deltaTime) override;
private:
    std::weak_ptr<TileMap> tileMapWeak;
    std::weak_ptr<WayFindingTank> tankWeak;
};
