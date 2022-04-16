#include <SpriteGroup.h>
#include <Scene.h>
#include <iostream>

class SpriteGroupData
{
public:
    SpriteGroupID groupID;
    std::list<SpritePointer> sprites;
    Scene* scene = nullptr;
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

void SpriteGroup::removeDeathSprite()
{
    data->sprites.remove_if([&](SpritePointer sprite) {
        if(sprite->isActive())
            return false;
        data->scene->removeChild(sprite);
        return true;
    });
}

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
