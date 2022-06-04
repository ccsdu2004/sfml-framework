#pragma once
#include <StateMachine.h>
#include <QuadTreeScene.h>
#include <TileMap.h>
#include <Text.h>
#include <MovingSprite.h>
#include "WayFindingTank.h"
#include "WayFindingTileVisitor.h"
#include "WayFindingState.h"

static inline const int TILE_ID_OFFSET = 10000;

inline size_t getTileIDByPosition(int x, int y)
{
    return x * TILE_ID_OFFSET + y;
}

class MineSweeperScene : public QuadTreeScene
{
public:
    MineSweeperScene();
public:
    void initial() override;
private:
    void onUpdateMyScene(float deltaTime) override;
private:
    std::shared_ptr<TileMap> tileMap;
    std::shared_ptr<WayFindingTank> tank;
    std::shared_ptr<WayFindingTileVisitor> wayFindingTileVisitor;

    std::shared_ptr<WayFindingState> wayFindingState;
    std::shared_ptr<StateMachine> shortPathStateMachine;
};
