#pragma once
#include <memory>
#include <Def.h>

enum SpriteDeleter_Type {
    SpriteDeleter_Direct = 0,
    SpriteDeleter_Slop,
    SpriteDeleter_Max
};

class SpriteDeleter
{
public:
    static std::shared_ptr<SpriteDeleter> create(SpriteDeleter_Type deleteType);
    virtual void setScene(SceneWeakPointer scene) = 0;
    virtual bool shouldDelete(SpritePointer sprite)const = 0;
};
