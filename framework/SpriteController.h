#pragma once
#include <memory>
#include <Def.h>

class SpriteController
{
public:
    virtual void setScene(SceneWeakPointer scene) = 0;
    virtual void updateSprite(SpritePointer sprite, float deltaTime) = 0;
};
