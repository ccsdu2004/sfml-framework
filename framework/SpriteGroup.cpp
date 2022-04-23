#include <SpriteGroup.h>
#include <Scene.h>
#include <iostream>

class SpriteGroupData
{
public:
    SpriteGroupID groupID;
    std::list<SpritePointer> sprites;
    Scene* scene;
};

SpriteGroup::SpriteGroup(SpriteGroupID id):
    data(new SpriteGroupData)
{
    data->groupID = id;
}

SpriteGroup::~SpriteGroup()
{
}

void SpriteGroup::setScene(Scene *scene)
{
    data->scene = scene;
}

SpriteGroupID SpriteGroup::getSpriteGroupID() const
{
    return data->groupID;
}

void SpriteGroup::addSprite(SpritePointer sprite)
{
    data->sprites.push_back(sprite);
}

void SpriteGroup::removeSprite(SpritePointer sprite)
{
    data->sprites.remove(sprite);
}

/*
void SpriteGroup::removeDeathSprite()
{
    auto itr = data->sprites.begin();
    while(itr != data->sprites.end()) {
        std::cout << (*itr)->getSpriteStatus() << std::endl;
        itr ++;
    }

    data->sprites.remove_if([&](SpritePointer sprite) {
        if(sprite->getSpriteStatus() == SpriteStatus_Death)
            return false;
        data->scene->removeChild(sprite);
        return true;
    });
}*/

void SpriteGroup::accept(ObjectVisitor *visitor)
{
    if(visitor) {
        auto itr = data->sprites.begin();
        while(itr != data->sprites.end()) {
            auto object = *itr;
            visitor->visitObject<Sprite>(object);
            itr ++;
        }
    }
}
