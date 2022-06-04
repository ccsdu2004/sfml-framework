#include <Application.h>
#include "SpriteForwardState.h"
#include "WayFindingTank.h"

WayFindingTank::WayFindingTank(const std::string &image, float x, float y)
{
    setPosition(x, y);
    //setSpriteStatus(SpriteStatus_Normal);
    setScale(0.3f);
    auto texture = Application::getInstance()->loadTexture(image);
    assert(texture);
    addTexture(*texture);
    auto size = texture->getSize();
    setSize(size.x, size.y);
}

void WayFindingTank::init(TileMapPointer tileMap)
{
    this->tileMap = tileMap;

    tankStateMachine = std::make_shared<StateMachine>();

    errorState = std::make_shared<SpriteErrorState>();
    idleState = std::make_shared<SpriteIdleState>(tileMap, shared_from_this());
    forwardState = std::make_shared<SpriteForwardState>(tileMap, shared_from_this());
    turnLeftState = std::make_shared<SpriteTurnLeftState>(shared_from_this());
    turnRightState = std::make_shared<SpriteTurnRightState>(shared_from_this());

    tankStateMachine->addState(errorState);
    tankStateMachine->addState(idleState);
    tankStateMachine->addState(forwardState);
    tankStateMachine->addState(turnLeftState);
    tankStateMachine->addState(turnRightState);
    tankStateMachine->setInitState(idleState);
    tankStateMachine->setErrorState(errorState);

    tankStateMachine->addTransition(forwardState, idleState, []()->bool{return true;}, 100);
    tankStateMachine->addTransition(turnLeftState, idleState, []()->bool{return true;}, 100);
    tankStateMachine->addTransition(turnRightState, idleState, []()->bool{return true;}, 100);

    tankStateMachine->start();
}

void WayFindingTank::makeForward()
{
    if (tankStateMachine->getCurrentState() != idleState)
        return;

    auto message = std::make_shared<StateSwitchMessage>("forward",
                   std::bind(&WayFindingTank::shouldForward, this));
    tankStateMachine->process(message);
}

void WayFindingTank::makeTurnLeft()
{
    if (tankStateMachine->getCurrentState() != idleState)
        return;

    auto message = std::make_shared<StateSwitchMessage>("turnleft");
    tankStateMachine->process(message);
}

void WayFindingTank::makeTurnRight()
{
    if (tankStateMachine->getCurrentState() != idleState)
        return;

    auto message = std::make_shared<StateSwitchMessage>("turnright");
    tankStateMachine->process(message);
}

std::shared_ptr<State> WayFindingTank::getCurrentState() const
{
    return tankStateMachine->getCurrentState();
}

bool WayFindingTank::shouldForward()
{
    auto position = getPosition();
    auto tileIndex = tileMap.lock()->getTileIndexByWorldPosition(position.x, position.y);
    int direction = getRotate();
    auto targetTileIndex = tileMap.lock()->getAdjacentTileByDirection(tileIndex.x, tileIndex.y,
                           (TileDirection)direction);
    if (targetTileIndex.has_value()) {
        tileIndex = targetTileIndex.value();
        return nullptr != tileMap.lock()->getTileByIndex(tileIndex.x, tileIndex.y);
    }
    return false;
}

void WayFindingTank::onUpdateMovingSprite(float deltaTime)
{
    tankStateMachine->update(deltaTime);
}
