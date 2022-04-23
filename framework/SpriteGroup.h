#pragma once
#include <NameHolder.h>
#include <Sprite.h>

enum SpriteGroupID : uint32_t {
    SpriteGroupID_Bullet = 0,
    SpriteGroupID_PlayerA,
    SpriteGroupID_PlayerB,
    SpriteGroupID_PlayerC,
    SpriteGroupID_PlayerD,
    SpriteGroupID_Max
};

class SpriteGroup : public NameHolder
{
public:
    static SpriteGroupPointer createSpriteGroup(SpriteGroupID id)
    {
        return std::make_shared<SpriteGroup>(id);
    }

    SpriteGroup() = delete;
    SpriteGroup(SpriteGroupID id);
    virtual ~SpriteGroup();
public:
    void setScene(Scene* scene);

    SpriteGroupID getSpriteGroupID()const;
    void addSprite(SpritePointer sprite);
    void removeSprite(SpritePointer sprite);

    void accept(ObjectVisitor* visitor);
private:
    std::unique_ptr<class SpriteGroupData> data;
};


