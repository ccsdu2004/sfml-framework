#pragma once
#include <set>
#include <Scene.h>
#include <Sprite.h>
#include <SpriteGroup.h>

class QuadTreeScene : public Scene
{
    friend class SpriteVisitor;
public:
    QuadTreeScene();
    virtual ~QuadTreeScene();
public:
    void addSpriteToGroup(SpritePointer sprite, SpriteGroupID group);
    virtual void onConllision(SpritePointer current, const std::set<SpritePointer>& sprites);
protected:
    void update(float deltaTime) override;
private:
    std::unique_ptr<class QuadTreeSceneData> data;
};
