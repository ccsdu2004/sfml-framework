#include <map>
#include <vector>
#include <QuadTree.h>
#include <QuadTreeScene.h>
#include <Application.h>
#include <Animation.h>
#include <SpriteDeleter.h>
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

    void visitObject(ObjectPointer object) override
    {
        auto sprite = std::dynamic_pointer_cast<Sprite>(object);
        if(sprite)
            quadTree->insert(sprite);
    }

    bool shouldDeleteSprite(ObjectPointer object)const
    {
        if(!object)
            return false;

        auto sprite = std::dynamic_pointer_cast<Sprite>(object);
        if(!sprite)
            return false;

        auto find = spriteDeleters.find(sprite->getSpriteGroup());
        if(find != spriteDeleters.end()) {
            auto list = find->second;
            for(auto itr = list.begin(); itr != list.end(); itr++) {
                auto spriteDeleter = *itr;
                if(spriteDeleter->shouldDelete(sprite))
                    return true;
            }
        }

        return false;
    }

    QuadTreeScene& scene;
    std::vector<SpriteGroupID> conllisionPair;
    std::shared_ptr<QuadTree> quadTree;
    std::map<SpriteGroupID, std::list<SpriteDeleterPointer>> spriteDeleters;
};

class SpriteSearchVisitor : public ObjectVisitor
{
public:
    SpriteSearchVisitor(QuadTreeScene &inputScene):
        scene(inputScene)
    {
    }

    void visitObject(ObjectPointer object) override
    {
        auto sprite = std::dynamic_pointer_cast<Sprite>(object);
        if(!sprite || sprite->getSpriteStatus() == SpriteStatus_Death)
            return;

        auto find = std::find(scene.data->conllisionPair.begin(), scene.data->conllisionPair.end(), sprite->getSpriteGroup());
        if(find == scene.data->conllisionPair.end())
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
}

QuadTreeScene::~QuadTreeScene()
{
}

void QuadTreeScene::addSpriteDeleter(SpriteGroupID groupID, SpriteDeleterPointer deleter)
{
    if(!deleter)
        return;

    deleter->setScene(std::dynamic_pointer_cast<Scene>(shared_from_this()));

    auto itr = data->spriteDeleters.find(groupID);
    if(itr != data->spriteDeleters.end())
        itr->second.push_back(deleter);
    else {
        auto group = groupID;
        auto node = std::make_pair<SpriteGroupID, std::list<SpriteDeleterPointer>>(std::move(group), {deleter});
        data->spriteDeleters.insert(node);
    }
}

void QuadTreeScene::onConllision(SpritePointer current, const std::set<SpritePointer>& sprites)
{
    (void)current;
    (void)sprites;
}

void QuadTreeScene::onUpdateObject(float deltaTime)
{
    data->quadTree->clear();
    Scene::onUpdateObject(deltaTime);

    acceptObject(data.get());

    SpriteSearchVisitor visitor(*this);
    acceptObject(&visitor);

    auto deleter = std::bind(&QuadTreeSceneData::shouldDeleteSprite, data.get(), std::placeholders::_1);
    removeChild(deleter);
}

void QuadTreeScene::addConllisionGroupID(SpriteGroupID groupID)
{
    data->conllisionPair.push_back(groupID);
}
