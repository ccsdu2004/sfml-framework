#pragma once
#include <list>
#include <memory>
#include <Sprite.h>
#include <Component.h>

template<class T>
class SpritePool : public Component
{
public:
    SpritePool()
    {
        static_assert (std::is_base_of<Sprite, T>() && "T must be derived of Sprite");
    }
public:
    std::shared_ptr<T> createOrAwakeSprite()
    {
        auto itr = sprites.begin();
        while (itr != sprites.end()) {
            auto sprite = *itr;
            if (sprite->getSpriteStatus() == SpriteStatus::SpriteStatus_Death) {
                sprite->setSpriteStatus(SpriteStatus::SpriteStatus_Normal);
                return sprite;
            }
            itr ++;
        }

        return addSprite();
    }

    auto addSprite() -> std::shared_ptr<T>
    {
        auto sprite = std::make_shared<T>();
        sprites.push_back(sprite);
        return sprite;
    }
private:
    std::list<std::shared_ptr<T>> sprites;
};
