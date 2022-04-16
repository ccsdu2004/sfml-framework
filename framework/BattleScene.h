#pragma once
#include <Scene.h>
#include <Sprite.h>
#include <SpriteGroup.h>

class BattleScene : public Scene
{
    friend class SpriteVisitor;
public:
    BattleScene();
    virtual ~BattleScene();
public:
    void addSpriteToGroup(SpritePointer sprite, SpriteGroupID group);
    void update(const sf::Time& time) override;
private:
    std::unique_ptr<class BattleSceneData> data;
};
