#include "SpriteIdleState.h"

SpriteIdleState::SpriteIdleState(std::shared_ptr<TileMap> inputTileMap,
                                 ObjectPointer inputTank):
    State(inputTank)
{
    setName("idle");
    tileMapWeak = inputTileMap;
    tankWeak = std::dynamic_pointer_cast<WayFindingTank>(inputTank);
}

void SpriteIdleState::onEnter()
{
    //std::cout << "enter idel" << std::endl;
}

void SpriteIdleState::onLeave()
{
    // std::cout << "enter leave" << std::endl;
}

void SpriteIdleState::update(float deltaTime)
{
    (void)deltaTime;
    setFinished();
}

