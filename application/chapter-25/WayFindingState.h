#pragma once
#include <StateMachine.h>
#include <TileMap.h>
#include "WayFindingTank.h"

class WayFindingTileVisitor;

class WayFindingState : public State
{
public:
    WayFindingState() = delete;
    WayFindingState(std::shared_ptr<TileMap> inputTileMap, std::shared_ptr<WayFindingTank> inputTank, std::shared_ptr<WayFindingTileVisitor> inputVisitor);
public:
    void onEnter() override;
    void onLeave() override;
    void update(float deltaTime) override;
public:
    std::optional<int> getRotate();
private:
    std::weak_ptr<TileMap> tileMapWeak;
    std::weak_ptr<WayFindingTank> tankWeak;
    std::weak_ptr<WayFindingTileVisitor> tileVisitor;
};

class WayFindingErrorState : public State
{
public:
    WayFindingErrorState() = delete;
    WayFindingErrorState(std::shared_ptr<TileMap> inputTileMap, std::shared_ptr<WayFindingTank> inputTank);
public:
    void onEnter() override;
    void onLeave() override;
    void update(float deltaTime) override;
private:
    std::weak_ptr<TileMap> tileMap;
    std::weak_ptr<WayFindingTank> tank;
};
