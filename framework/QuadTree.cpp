#include <vector>
#include <map>
#include <QuadTree.h>

#define LT 0
#define RT 1
#define RB 2
#define LB 3

class QuadTreeData
{
public:
    QuadTreeData(QuadTree *inputTree, const sf::FloatRect &inputBox,
                 uint32_t inputDepth,
                 uint32_t inputMaxDepth):
        depth(inputDepth),
        maxDepth(inputMaxDepth)
    {
        box = inputBox;
        tree = inputTree;
    }

    void clear()
    {
        for (int i = 0; i < 4; i++)
            if (leaf[i]) {
                leaf[i]->clear();
                leaf[i].reset();
            }
        tree = nullptr;
        objects.clear();
    }

    bool isContainLT(const sf::FloatRect &input)
    {
        float x = box.left;
        float y = box.top;
        float width = box.width;
        float height = box.height;

        auto lt = sf::FloatRect(x, y, width / 2.0f, height / 2.0f);
        return lt.intersects(input);
    }

    void addLT(SpritePointer item)
    {
        float x = box.left;
        float y = box.top;
        float width = box.width;
        float height = box.height;

        auto area = sf::FloatRect(x, y, width / 2.0f, height / 2.0f);
        if (!leaf[LT])
            leaf[LT] = std::make_shared<QuadTree>(area, depth + 1,
                                                  maxDepth);
        leaf[LT]->insert(item);
    }

    bool isContainRT(const sf::FloatRect &input)
    {
        float x = box.left;
        float y = box.top;
        float width = box.width;
        float height = box.height;

        auto rt = sf::FloatRect(x + width / 2.0f, y, width / 2.0f, height / 2.0f);
        return rt.intersects(input);
    }

    void addRT(SpritePointer item)
    {
        float x = box.left;
        float y = box.top;
        float width = box.width;
        float height = box.height;

        auto area = sf::FloatRect(x + width / 2.0f, y, width / 2.0f,
                                  height / 2.0f);

        if (!leaf[RT])
            leaf[RT] = std::make_shared<QuadTree>(area, depth + 1,
                                                  maxDepth);
        leaf[RT]->insert(item);
    }

    bool isContainRB(const sf::FloatRect &input)
    {
        float x = box.left;
        float y = box.top;
        float width = box.width;
        float height = box.height;

        auto rb = sf::FloatRect(x + width / 2.0f, y + height / 2.0f, width / 2.0f, height / 2.0f);
        return rb.intersects(input);
    }

    void addRB(SpritePointer item)
    {
        float x = box.left;
        float y = box.top;
        float width = box.width;
        float height = box.height;

        auto area = sf::FloatRect(x + width / 2.0f, y + height / 2.0f,
                                  width / 2.0f,
                                  height / 2.0f);

        if (!leaf[RB])
            leaf[RB] = std::make_shared<QuadTree>(area,
                                                  depth + 1,
                                                  maxDepth);

        leaf[RB]->insert(item);
    }

    bool isContainLB(const sf::FloatRect &input)
    {
        float x = box.left;
        float y = box.top;
        float width = box.width;
        float height = box.height;

        auto lb = sf::FloatRect(x, y + height / 2.0f, width / 2.0f, height / 2.0f);
        return lb.intersects(input);
    }

    void addLB(SpritePointer item)
    {
        float x = box.left;
        float y = box.top;
        float width = box.width;
        float height = box.height;

        auto area =  sf::FloatRect(x, y + height / 2.0f, width / 2.0f,
                                   height / 2.0f);

        if (!leaf[LB])
            leaf[LB] = std::make_shared<QuadTree>(area,
                                                  depth + 1,
                                                  maxDepth);

        leaf[LB]->insert(item);
    }

    sf::FloatRect box;
    uint32_t depth;
    uint32_t maxDepth;
    QuadTree *tree = nullptr;
    std::shared_ptr<QuadTree> leaf[4];
    std::set<SpritePointer> objects;
};

class QuadTreeObjectVisitor : public QuadTreeVisitor
{
public:
    QuadTreeObjectVisitor(const sf::FloatRect &inputArea):
        area(inputArea)
    {
    }
public:
    void visit(QuadTree *tree)
    {
        if (area.intersects(tree->getBox()) && tree->getChildTree().empty()) {
            auto list = tree->getObjects();
            std::for_each(list.begin(), list.end(), [&](SpritePointer object) {
                foundObjects.insert(object);
            });
        }
    }

    std::set<SpritePointer> getFoundObjects()const
    {
        return foundObjects;
    }
private:
    std::map<uint32_t, std::list<QuadTree *>> nodes;
    sf::FloatRect area;
    std::set<SpritePointer> foundObjects;
};

QuadTree::QuadTree(const sf::FloatRect &area, uint32_t depth, uint32_t maxDepth):
    data(new QuadTreeData(this, area, depth, maxDepth))
{
}

QuadTree::~QuadTree()
{
}

void QuadTree::accept(QuadTreeVisitor *visitor)
{
    if (visitor) {
        visitor->visit(this);

        for (int i = 0; i < 4; i++)
            if (data->leaf[i])
                data->leaf[i]->accept(visitor);
    }
}

sf::FloatRect QuadTree::getBox() const
{
    return data->box;
}

std::list<QuadTree *> QuadTree::getChildTree() const
{
    std::list<QuadTree *> children;
    for (int i = 0; i < 4; i++)
        if (data->leaf[i])
            children.push_back(data->leaf[i].get());
    return children;
}

uint32_t QuadTree::getMaxDepth() const
{
    return data->maxDepth;
}

uint32_t QuadTree::getDepth() const
{
    return data->depth;
}

void QuadTree::clear()
{
    data->clear();
}

void QuadTree::insert(SpritePointer item)
{
    if (data->depth == data->maxDepth) {
        data->objects.insert(item);
        return;
    }

    bool find[4] = {false};

    sf::FloatRect box(item->getPosition(), item->getSize());

    if (data->isContainLT(box)) {
        find[LT] = true;
        data->addLT(item);
    }

    if (data->isContainRT(box)) {
        find[RT] = true;
        data->addRT(item);
    }

    if (data->isContainLB(box)) {
        find[LB] = true;
        data->addLB(item);
    }

    if (data->isContainRB(box)) {
        find[RB] = true;
        data->addRB(item);
    }

    if (find[LT] || find[RT] || find[LB] || find[RB])
        data->objects.insert(item);
}

std::set<SpritePointer> QuadTree::getObjects() const
{
    return data->objects;
}

std::set<SpritePointer> QuadTree::search(const sf::FloatRect &area)
{
    std::unique_ptr<QuadTreeObjectVisitor> visitor(new QuadTreeObjectVisitor(area));
    accept(visitor.get());
    return visitor->getFoundObjects();
}

QuadTreeVisitor::~QuadTreeVisitor()
{
}

