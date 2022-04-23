#include <map>
#include <array>
#include <QuadTree.h>
#include <BattleScene.h>
#include <SpriteGroup.h>
#include <Application.h>
#include <Animation.h>
#include <iostream>

class SpriteVisitor;

class BattleSceneData : public ObjectVisitor
{
public:
    BattleSceneData(BattleScene *inputScene):
        scene(inputScene)
    {
        auto windowSize = Application::getInstance()->getWindow()->getSize();
        quadTree = std::make_shared<QuadTree>(sf::FloatRect(0.0f, 0.0f, windowSize.x, windowSize.y), 0, 5);
    }

    void visit(SpritePointer sprite) override
    {
        quadTree->insert(sprite);
    }

    void updateSprite();

    BattleScene *scene = nullptr;
    std::map<SpriteGroupID, SpriteGroupPointer> spriteGroups;
    std::shared_ptr<QuadTree> quadTree;
};

class SpriteVisitor : public ObjectVisitor
{
public:
    SpriteVisitor(BattleScene &inputScene):
        scene(inputScene)
    {
    }

    void visit(SpritePointer sprite) override
    {
        auto quadTree = scene.data.get()->quadTree;
        auto items = quadTree->search(sprite->getBoundingBox());
        if (items.size() > 1) {
            for (auto item : items) {
                item->setSpriteStatus(SpriteStatus_Death);

                std::shared_ptr<Animation> animation = std::make_shared<Animation>();
                animation->setSingleShot(false);
                animation->setPosition(sprite->getPosition());

                std::vector<sf::IntRect> areas;

                for (int i = 0; i < 6; i++) {
                    auto area = sf::IntRect(i * 85, 0, 85, 85);
                    areas.push_back(area);
                }

                animation->setTexture("../resource/images/blast2.png", areas);
                scene.addChild(animation);
            }
        }
    }
private:
    BattleScene &scene;
};

BattleScene::BattleScene():
    data(new BattleSceneData(this))
{
    for (auto i = 0; i != SpriteGroupID::SpriteGroupID_Max; i++) {
        auto group = SpriteGroup::createSpriteGroup(static_cast<SpriteGroupID>(i));
        group->setScene(this);
        data->spriteGroups.insert(std::make_pair(static_cast<SpriteGroupID>(i), group));
    }
}

BattleScene::~BattleScene()
{
}

void BattleScene::addSpriteToGroup(SpritePointer sprite, SpriteGroupID group)
{
    assert(data->spriteGroups.find(group) != data->spriteGroups.end());
    auto spriteGroup = data->spriteGroups[group];
    spriteGroup->addSprite(sprite);
    sprite->setSpriteGroup(spriteGroup);
    addChild(sprite);
}

void BattleScene::update(float deltaTime)
{
    data->quadTree->clear();
    Scene::update(deltaTime);
    accept(data.get());

    data->updateSprite();
}

void BattleSceneData::updateSprite()
{
    {
        auto spriteGroup = spriteGroups[SpriteGroupID::SpriteGroupID_PlayerB];
        SpriteVisitor visitor(*scene);
        spriteGroup->accept(&visitor);
        spriteGroup->removeDeathSprite();
    }
    {
        auto spriteGroup = spriteGroups[SpriteGroupID::SpriteGroupID_Bullet];
        spriteGroup->removeDeathSprite();
    }
}
