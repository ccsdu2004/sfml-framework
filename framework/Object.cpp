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
    std::map<uint32_t, std::any> data;
};

Object::Object():
    data(new ObjectData())
{
}

Object::~Object()
{
}

void Object::setData(uint32_t key, const std::any &value)
{
    data->data.insert(std::make_pair(key, value));
}

std::any Object::getData(uint32_t key) const
{
    auto itr = data->data.find(key);
    if(itr == data->data.end())
        return std::any();
    return itr->second;
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
    if(!visitor)
        return;

    auto itr = data->children.begin();
    while(itr != data->children.end()) {
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
    while(itr != data->children.end()) {
        auto child = *itr;
        if(child->process(message))
            return true;
        itr ++;
    }
    return MessageReceiver::process(message);
}

void Object::update(const sf::Time &time)
{
    auto itr = data->children.begin();
    while(itr != data->children.end()) {
        auto child = *itr;
        child->update(time);
        itr ++;
    }
}

void Object::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    auto itr = data->children.begin();
    while(itr != data->children.end()) {
        auto child = *itr;
        child->draw(target, states);
        itr ++;
    }
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

