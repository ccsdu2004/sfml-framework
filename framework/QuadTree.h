#pragma once
#include <set>
#include <memory>
#include <list>
#include <SFML/Graphics/Rect.hpp>
#include <Component.h>
#include <Sprite.h>

class QuadTreeVisitor;

class QuadTree : public Component
{
    friend class QuadTreeData;
public:
    QuadTree() = delete;
    QuadTree(const sf::FloatRect &area, uint32_t depth = 0, uint32_t maxDepth = 5);
    ~QuadTree();
public:
    void accept(QuadTreeVisitor *visitor);
    void update(float deltaTime) override;

    sf::FloatRect getBox()const;
    std::list<QuadTree *> getChildTree()const;

    uint32_t getMaxDepth()const;
    uint32_t getDepth()const;

    void clear();
    void insert(SpritePointer item);

    std::set<SpritePointer> getObjects()const;
    std::set<SpritePointer> search(const sf::FloatRect &area);
private:
    std::unique_ptr<class QuadTreeData> data;
};

class QuadTreeVisitor
{
public:
    virtual ~QuadTreeVisitor();
public:
    virtual void visit(QuadTree *tree) = 0;
};
