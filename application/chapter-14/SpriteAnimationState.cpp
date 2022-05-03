#include <MovingSprite.h>
#include "MineSweeperScene.h"
#include "SpriteAnimationState.h"

void SpriteAnimationState::onEnter()
{
    if(getTarget().expired())
        return;

    if(scene.expired())
        return;

    auto tileMap = scene.lock()->getTileMap();
    auto sprite = std::dynamic_pointer_cast<Sprite>(getTarget().lock());
    auto spritePosition = sprite->getPosition();
    auto tileIndex = tileMap->getTileIndexByWorldPosition(spritePosition.x, spritePosition.y);
    tileID = getTileIDByPosition(tileIndex.x, tileIndex.y);

    animation->setPosition(sprite->getPosition().x * 0.5, 0.5 * sprite->getPosition().y);
    sprite->addChild(animation);
    animation->start();
}

void SpriteAnimationState::onLeave()
{
    getTarget().lock()->removeChild(animation);
    if(scene.expired())
        return;
    scene.lock()->removeMine(tileID);
}

AnimationPointer SpriteAnimationState::createAnimation()
{
    std::vector<sf::IntRect> areas;

    for(int i = 0; i < 6; i++) {
        auto area = sf::IntRect(i * 85, 0, 85, 85);
        areas.push_back(area);
    }

    std::shared_ptr<Animation> animation = std::make_shared<Animation>();
    animation->setDurationPerFrame(0.2f);
    animation->setSingleShot(true);

    animation->setBackgroundColor(sf::Color::White);
    animation->setTexture("../resource/images/blast2.png", areas);
    return animation;
}
