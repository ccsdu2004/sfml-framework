#pragma once
#include <StateMachine.h>
#include <QuadTreeScene.h>
#include <TileMap.h>
#include <Text.h>
#include <MovingSprite.h>

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
    std::shared_ptr<TileMap> getTileMap()const;
public:
    void makeSpriteForward();
    void makeSpriteTurnLeft();
    void makeSpriteTurnRight();
private:
    std::shared_ptr<MovingSprite> createSprite(const std::string &image, float x, float y);

    void onUpdateMyScene(float deltaTime) override;
private:
    bool shouldForward();
private:
    std::shared_ptr<TileMap> tileMap;
    std::shared_ptr<MovingSprite> mineSweeper;
    std::shared_ptr<StateMachine> stateMachine;

    std::shared_ptr<State> errorState;
    std::shared_ptr<State> idleState;

    std::shared_ptr<State> forwardState;
    std::shared_ptr<State> turnLeftState;
    std::shared_ptr<State> turnRightState;
};
