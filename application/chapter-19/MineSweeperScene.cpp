#include <Application.h>
#include <ResourceManager.h>
#include "MineSweeperScene.h"
#include "SpriteErrorState.h"
#include "SpriteIdleState.h"
#include "SpriteAnimationState.h"
#include "SpriteForwardState.h"
#include "SpriteRotateState.h"
#include "InputMessageListener.h"

MineSweeperScene::MineSweeperScene()
{
    auto listener = std::make_shared<InputMessageListener>(*this);
    addMessageListener(listener);

    tileMap = TileMap::createTileMap(TileMapType_Hex);
    tileMap->init(20, 14, 64);
    tileMap->setTextVisible(true);

    addChild(tileMap);
}

void MineSweeperScene::initial()
{
    auto position = tileMap->getTileByIndex(8, 6)->getPosition();
    mineSweeper = createSprite("../resource/images/tank/tank5.png", position.x,
                               position.y);
    addChild(mineSweeper);

    stateMachine = std::make_shared<StateMachine>();

    errorState = std::make_shared<SpriteErrorState>();
    idleState = std::make_shared<SpriteIdleState>();
    forwardState = std::make_shared<SpriteForwardState>(tileMap, mineSweeper);
    turnLeftState = std::make_shared<SpriteTurnLeftState>(mineSweeper);
    turnRightState = std::make_shared<SpriteTurnRightState>(mineSweeper);

    stateMachine->addState(errorState);
    stateMachine->addState(idleState);
    stateMachine->addState(forwardState);
    stateMachine->addState(turnLeftState);
    stateMachine->addState(turnRightState);
    stateMachine->setInitState(idleState);
    stateMachine->setErrorState(errorState);

    stateMachine->addTransition(forwardState, idleState, []()->bool{return true;}, 100);
    stateMachine->addTransition(turnLeftState, idleState, []()->bool{return true;}, 100);
    stateMachine->addTransition(turnRightState, idleState, []()->bool{return true;}, 100);

    stateMachine->start();

    auto text = createToastText();
    text->setText(L"事件驱动状态机", false);
    addChild(text);
}

std::shared_ptr<MovingSprite> MineSweeperScene::createSprite(const std::string &image, float x,
        float y)
{
    auto sprite = std::make_shared<MovingSprite>();
    sprite->setPosition(x, y);
    sprite->setSpriteStatus(SpriteStatus_Normal);
    sprite->setScale(0.3f);
    auto textureManager = Application::getInstance()->getComponent<ResourceManager<sf::Texture>>();

    auto texture = textureManager->loadFromFile(image);
    sprite->addTexture(*texture);
    auto size = texture->getSize();
    sprite->setSize(size.x, size.y);
    return sprite;
}

void MineSweeperScene::onUpdateMyScene(float deltaTime)
{
    stateMachine->update(deltaTime);
}

std::shared_ptr<TileMap> MineSweeperScene::getTileMap() const
{
    return tileMap;
}

void MineSweeperScene::makeSpriteForward()
{
    if (stateMachine->getCurrentState() != idleState)
        return;

    auto message = std::make_shared<StateSwitchMessage>("forward",
                   std::bind(&MineSweeperScene::shouldForward, this));
    stateMachine->process(message);
}

void MineSweeperScene::makeSpriteTurnLeft()
{
    if (stateMachine->getCurrentState() != idleState)
        return;

    auto message = std::make_shared<StateSwitchMessage>("turnleft");
    stateMachine->process(message);
}

void MineSweeperScene::makeSpriteTurnRight()
{
    if (stateMachine->getCurrentState() != idleState)
        return;

    auto message = std::make_shared<StateSwitchMessage>("turnright");
    stateMachine->process(message);
}

bool MineSweeperScene::shouldForward()
{
    auto position = mineSweeper->getPosition();
    auto tileIndex = tileMap->getTileIndexByWorldPosition(position.x, position.y);
    int direction = mineSweeper->getRotate();
    auto targetTileIndex = tileMap->getAdjacentTileByDirection(tileIndex.x, tileIndex.y,
                           (TileDirection)direction);
    if (targetTileIndex.has_value()) {
        tileIndex = targetTileIndex.value();
        return nullptr != tileMap->getTileByIndex(tileIndex.x, tileIndex.y);
    }
    return false;
}
