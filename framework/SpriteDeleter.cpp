#include <Sprite.h>
#include <Scene.h>
#include <SpriteDeleter.h>
#include <Application.h>
#include <iostream>

class SpriteDerectyDeleter : public SpriteDeleter
{
public:
    void setScene(SceneWeakPointer scene) override
    {
        (void)scene;
    }

    bool shouldDelete(SpritePointer sprite)const override
    {
        return sprite && sprite->getSpriteStatus() == SpriteStatus_Death;
    }
};

class SpriteSlopDeleter : public SpriteDeleter
{
public:
    void setScene(SceneWeakPointer inputScene) override
    {
        scene = inputScene;
    }

    bool shouldDelete(SpritePointer sprite)const override
    {
        if(!sprite)
            return false;
        if(scene.expired())
            return false;
        bool ret = !sprite->getBoundingBox().intersects(scene.lock()->getBoundingBox());
        if(ret)
            sprite->setSpriteStatus(SpriteStatus_Death);
        return ret;
    }
private:
    SceneWeakPointer scene;
};

std::shared_ptr<SpriteDeleter> SpriteDeleter::create(SpriteDeleter_Type deleteType)
{
    if (deleteType == SpriteDeleter_Direct)
        return std::make_shared<SpriteDerectyDeleter>();
    else if (deleteType == SpriteDeleter_Slop)
        return std::make_shared<SpriteSlopDeleter>();
    return nullptr;
}
