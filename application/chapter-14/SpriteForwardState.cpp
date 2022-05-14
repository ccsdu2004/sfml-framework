#include <Util.h>
#include "SpriteForwardState.h"

SpriteForwardState::SpriteForwardState(std::shared_ptr<TileMap> inputTileMap, ObjectPointer inputSprite):
    State(inputSprite),
    tileMap(inputTileMap)
{
    setName("forward");
}

void SpriteForwardState::onEnter()
{
    std::cout << "enter forward" << std::endl;
    if(getTarget().expired())
        return;

    auto sprite = std::dynamic_pointer_cast<MovingSprite>(getTarget().lock());
    auto spritePosition = sprite->getPosition();

    if(tileMap.expired())
        return;

    auto tileMapPointer = tileMap.lock();
    auto tileIndex = tileMapPointer->getTileIndexByWorldPosition(spritePosition.x, spritePosition.y);
    std::optional<sf::Vector2i> option = tileMap.lock()->getAdjacentTileByDirection(tileIndex.x, tileIndex.y, (TileDirection)(int)sprite->getRotate());
    if(option.has_value()) {
        tileIndex = option.value();
        auto tile = tileMapPointer->getTileByIndex(tileIndex.x, tileIndex.y);
        tile->setFillColor(sf::Color::Blue);
        targetPosition = tile->getPosition();

        auto diffPosition = targetPosition - spritePosition;
        sprite->setVelocity(sf::Vector2f(diffPosition.x * FORWARD_SPEED, diffPosition.y * FORWARD_SPEED));
    }
}

void SpriteForwardState::onLeave()
{
    if(getTarget().expired())
        return;

    auto sprite = std::dynamic_pointer_cast<MovingSprite>(getTarget().lock());
    sprite->setVelocity(sf::Vector2f(0, 0));
    sprite->setPosition(targetPosition);
}

void SpriteForwardState::update(float deltaTime)
{
    (void)deltaTime;

    if(getTarget().expired())
        return;

    if(tileMap.expired())
        return;

    auto sprite = std::dynamic_pointer_cast<MovingSprite>(getTarget().lock());
    auto spritePosition = sprite->getPosition();

    float ds2 = distance2(targetPosition.x, targetPosition.y, spritePosition.x, spritePosition.y);
    if(ds2 < 1.0f)
        setFinished();
}
