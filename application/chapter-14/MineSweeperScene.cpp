#include <Application.h>
#include "MineSweeperScene.h"
#include "SpriteErrorState.h"
#include "SpriteAnimationState.h"
#include "SpriteForwardState.h"
#include "SpriteRotateState.h"

MineSweeperScene::MineSweeperScene()
{
    tileMap = TileMap::createTileMap(TileMapType_Hex);
    tileMap->init(18, 12, 36);
    tileMap->setTextVisible(true);

    addChild(tileMap);
    tileMap->accept(this);
}

void MineSweeperScene::initial()
{
    stateMachine = std::make_shared<StateMachine>();

    errorState = std::make_shared<SpriteErrorState>(mineSweeper);

    animationState = std::make_shared<SpriteAnimationState>(std::dynamic_pointer_cast<MineSweeperScene>(shared_from_this()), mineSweeper);
    animationState->setName("removemine");

    forwardState = std::make_shared<SpriteForwardState>(tileMap, mineSweeper);
    forwardState->setName("forward");

    turnLeftState = std::make_shared<SpriteTurnLeftState>(mineSweeper);
    turnLeftState->setName("turnleft");

    turnRightState = std::make_shared<SpriteTurnRightState>(mineSweeper);
    turnRightState->setName("turnright");

    stateMachine->addTransition(forwardState, animationState, std::bind(&MineSweeperScene::shouldRemovalMine, this), 100);
    stateMachine->addTransition(forwardState, turnLeftState, []()->bool{return true;}, 30);
    stateMachine->addTransition(forwardState, turnRightState, []()->bool{return true;}, 30);
    stateMachine->addTransition(forwardState, forwardState, std::bind(&MineSweeperScene::shouldForward, this), 40);

    stateMachine->addTransition(animationState, forwardState, std::bind(&MineSweeperScene::shouldForward, this), 60);
    stateMachine->addTransition(animationState, turnLeftState, []()->bool{return true;}, 30);
    stateMachine->addTransition(animationState, turnRightState, []()->bool{return true;}, 30);

    stateMachine->addTransition(turnLeftState, forwardState, std::bind(&MineSweeperScene::shouldForward, this), 40);
    stateMachine->addTransition(turnLeftState, turnLeftState, []()->bool{return true;}, 30);
    stateMachine->addTransition(turnLeftState, turnRightState, []()->bool{return true;}, 30);

    stateMachine->addTransition(turnRightState, forwardState, std::bind(&MineSweeperScene::shouldForward, this), 40);
    stateMachine->addTransition(turnRightState, turnLeftState, []()->bool{return true;}, 30);
    stateMachine->addTransition(turnRightState, turnRightState, []()->bool{return true;}, 30);

    if(shouldRemovalMine())
        stateMachine->setInitState(animationState);
    else
        stateMachine->setInitState(forwardState);

    stateMachine->setErrorState(errorState);
    stateMachine->start();
}

void MineSweeperScene::visit(uint32_t x, uint32_t y, std::shared_ptr<Tile> tile)
{
    if(x == 0 || x == 17)
        return;

    int flag = rand() % 3;
    if(flag == 0) {
        addMine(x, y, tile);
    }

    if(x == 8 && y == 6 && !mineSweeper) {
        mineSweeper = createSprite("../resource/images/tank.png", tile->getPosition().x, tile->getPosition().y);
        currentTile.x = x;
        currentTile.y = y;
        addChild(mineSweeper);
    }
}

void MineSweeperScene::addMine(uint32_t x, uint32_t y, std::shared_ptr<Tile> tile)
{
    auto sprite = createSprite("../resource/images/cross.png", tile->getPosition().x, tile->getPosition().y); // size.x * 0.5f, 600);
    sprite->setScale(0.5f);
    sprite->setSpriteGroup(SpriteGroupID_PlayerA);
    sprite->setSpriteColor(sf::Color::Red);
    mines.insert(std::make_pair(getTileIDByPosition(x, y), sprite));
    addChild(sprite);
}

std::shared_ptr<MovingSprite> MineSweeperScene::createSprite(const std::string &image, float x, float y)
{
    auto sprite = std::make_shared<MovingSprite>();
    sprite->setPosition(x, y);
    sprite->setSpriteStatus(SpriteStatus_Normal);
    sprite->setScale(0.5f);
    auto texture = Application::getInstance()->loadTexture(image);
    sprite->addTexture(*texture);
    auto size = texture->getSize();
    sprite->setSize(size.x, size.y);
    return sprite;
}

void MineSweeperScene::onUpdateMyScene(float deltaTime)
{
    stateMachine->update(deltaTime);
}

void MineSweeperScene::removeMine(size_t tileID)
{
    auto find = mines.find(tileID);
    if(find != mines.end()) {
        removeChild(find->second);
        mines.erase(find);
    }
}

std::shared_ptr<TileMap> MineSweeperScene::getTileMap() const
{
    return tileMap;
}

bool MineSweeperScene::shouldRemovalMine()
{
    auto position = mineSweeper->getPosition();
    auto tileIndex = tileMap->getTileIndexByWorldPosition(position.x, position.y);
    auto id = getTileIDByPosition(tileIndex.x, tileIndex.y);
    return mines.find(id) != mines.end();
}

bool MineSweeperScene::shouldForward()
{
    auto position = mineSweeper->getPosition();
    auto tileIndex = tileMap->getTileIndexByWorldPosition(position.x, position.y);
    int direction = mineSweeper->getRotate();
    auto targetTileIndex = tileMap->getAdjacentTileByDirection(tileIndex.x, tileIndex.y, (TileDirection)direction);
    if(targetTileIndex.has_value()) {
        tileIndex = targetTileIndex.value();
        return nullptr != tileMap->getTileByIndex(tileIndex.x, tileIndex.y);
    }
    return false;
}
