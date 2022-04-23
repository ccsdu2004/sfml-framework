#include <map>
#include <array>
#include <QuadTree.h>
#include <QuadTreeScene.h>
#include <SpriteGroup.h>
#include <Application.h>
#include <Animation.h>
#include <iostream>

class SpriteVisitor;

class QuadTreeSceneData : public ObjectVisitor
{
public:
    QuadTreeSceneData(QuadTreeScene& inputScene):
        scene(inputScene)
    {
        auto windowSize = Application::getInstance()->getWindow()->getSize();
        quadTree = std::make_shared<QuadTree>(sf::FloatRect(0.0f, 0.0f, windowSize.x, windowSize.y), 0, 5);
    }

    void visit(SpritePointer sprite) override
    {
        quadTree->insert(sprite);
    }

    void visitSpriteGroup();

    QuadTreeScene& scene;
    std::map<SpriteGroupID, SpriteGroupPointer> spriteGroups;
    std::shared_ptr<QuadTree> quadTree;
};

class SpriteVisitor : public ObjectVisitor
{
public:
    SpriteVisitor(QuadTreeScene &inputScene):
        scene(inputScene)
    {
    }

    void visit(SpritePointer sprite) override
    {
        if(sprite->getSpriteStatus() == SpriteStatus_Death)
            return;

        auto quadTree = scene.data.get()->quadTree;
        auto items = quadTree->search(sprite->getBoundingBox());
        if (items.size() > 1)
            scene.onConllision(sprite, items);
    }
private:
    QuadTreeScene &scene;
};

QuadTreeScene::QuadTreeScene():
    data(new QuadTreeSceneData(*this))
{
    for (auto i = 0; i != SpriteGroupID::SpriteGroupID_Max; i++) {
        auto group = SpriteGroup::createSpriteGroup(static_cast<SpriteGroupID>(i));
        group->setScene(this);
        data->spriteGroups.insert(std::make_pair(static_cast<SpriteGroupID>(i), group));
    }
}

QuadTreeScene::~QuadTreeScene()
{
}

void QuadTreeScene::addSpriteToGroup(SpritePointer sprite, SpriteGroupID group)
{
    auto itr = data->spriteGroups.find(group);
    if(itr == data->spriteGroups.end())
        return;

    itr->second->addSprite(sprite);
    sprite->setSpriteGroup(itr->second);
    addChild(sprite);
}

void QuadTreeScene::onConllision(SpritePointer current, const std::set<SpritePointer>& sprites)
{
    (void)current;
    (void)sprites;
}

void QuadTreeScene::update(float deltaTime)
{
    data->quadTree->clear();
    Scene::update(deltaTime);
    accept(data.get());

    data->visitSpriteGroup();
}

void QuadTreeSceneData::visitSpriteGroup()
{
    for(uint32_t i = SpriteGroupID::SpriteGroupID_PlayerA; i < SpriteGroupID::SpriteGroupID_Max; i++) {
        auto spriteGroup = spriteGroups[SpriteGroupID(i)];
        SpriteVisitor visitor(scene);
        spriteGroup->accept(&visitor);
    }
}
