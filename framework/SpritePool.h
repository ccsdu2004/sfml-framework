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
    std::shared_ptr<T> findSrpte()
    {
        auto itr = sprites.begin();
        while (itr != sprites.end()) {
            if ((*itr)->getSpriteStatus() == SpriteStatus::SpriteStatus_Death)
                return *itr;
            itr ++;
        }

        auto sprite = std::make_shared<T>();
        sprite->setSpriteStatus(SpriteStatus::SpriteStatus_Death);
        sprites.push_back(sprite);
        return sprite;
    }
private:
    std::list<std::shared_ptr<T>> sprites;
};
