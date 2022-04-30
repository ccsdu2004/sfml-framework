#pragma once
#include <set>
#include <Scene.h>
#include <Sprite.h>

class QuadTreeScene : public Scene
{
    friend class SpriteVisitor;
    friend class SpriteSearchVisitor;
public:
    QuadTreeScene();
    virtual ~QuadTreeScene();
public:
    void addSpriteDeleter(SpriteGroupID groupID, SpriteDeleterPointer deleter);

    void addConllisionGroupID(SpriteGroupID groupID);
private:
    virtual void onConllision(SpritePointer current, const std::set<SpritePointer>& sprites);
    void onUpdateObject(float deltaTime) override;
private:
    std::unique_ptr<class QuadTreeSceneData> data;
};
