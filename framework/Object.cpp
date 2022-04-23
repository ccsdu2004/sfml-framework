#include <map>
#include <Object.h>
#include <Entity.h>
#include <Sprite.h>
#include <Switch.h>
#include <Animation.h>

class ObjectData
{
public:
    std::list<std::shared_ptr<Object>> children;
};

Object::Object():
    data(new ObjectData())
{
}

Object::~Object()
{
}

void Object::addChild(std::shared_ptr<Object> child)
{
    data->children.push_back(child);
}

void Object::removeChild(std::shared_ptr<Object> child)
{
    data->children.remove(child);
}

void Object::accept(ObjectVisitor *visitor)
{
    if (!visitor)
        return;

    auto itr = data->children.begin();
    while (itr != data->children.end()) {
        auto object = *itr;
        visitor->visit(object);
        visitor->visitObject<Entity>(object);
        visitor->visitObject<Sprite>(object);
        visitor->visitObject<Switch>(object);
        visitor->visitObject<Animation>(object);
        itr ++;
    }
}

bool Object::process(std::shared_ptr<Message> message)
{
    auto itr = data->children.begin();
    while (itr != data->children.end()) {
        auto child = *itr;
        if (child->process(message))
            return true;
        itr ++;
    }
    return MessageReceiver::process(message);
}

void Object::update(float deltaTime)
{
    auto itr = data->children.begin();
    while (itr != data->children.end()) {
        auto child = *itr;
        child->update(deltaTime);
        itr ++;
    }
}

void Object::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    auto entity = dynamic_cast<const Entity *>(this);
    if (entity)
        states.transform *= entity->getTransform();

    onDrawObject(target, states);

    auto itr = data->children.begin();
    while (itr != data->children.end()) {
        auto child = *itr;
        child->draw(target, states);
        itr ++;
    }
}

void Object::onDrawObject(sf::RenderTarget &target, sf::RenderStates states) const
{
}

ObjectVisitor::~ObjectVisitor()
{

}

void ObjectVisitor::visit(ObjectPointer object)
{
    (void)object;
}

void ObjectVisitor::visit(EntityPointer entity)
{
    (void)entity;
}

void ObjectVisitor::visit(SpritePointer sprite)
{
    (void)sprite;
}

void ObjectVisitor::visit(SwitchPointer switchObject)
{
    (void)switchObject;
}

void ObjectVisitor::visit(AnimationPointer animation)
{
    (void)animation;
}

