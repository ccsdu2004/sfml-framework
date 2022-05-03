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
    std::weak_ptr<Object> parent;
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

void Object::removeChild(std::function<bool (std::shared_ptr<Object>)> fn)
{
    data->children.remove_if(fn);
}

void Object::foreachChild(std::function<void (std::shared_ptr<Object>)> fn)
{
    std::for_each(data->children.begin(), data->children.end(), fn);
}

void Object::acceptObject(ObjectVisitor *visitor)
{
    if (!visitor)
        return;

    std::for_each(data->children.begin(), data->children.end(), [visitor](std::shared_ptr<Object> object) {
        visitor->visitObject(object);
    });
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
    onUpdateChildren(deltaTime);
    onUpdateObject(deltaTime);
}

void Object::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    auto entity = dynamic_cast<const Entity *>(this);
    if (entity)
        states.transform *= entity->getTransform();

    onDrawObject(target, states);

    std::for_each(data->children.begin(), data->children.end(), [&target, states](std::shared_ptr<Object> object) {
        object->draw(target, states);
    });
}

void Object::onDrawObject(sf::RenderTarget &target, sf::RenderStates states) const
{
    (void)target;
    (void)states;
}

void Object::onUpdateChildren(float deltaTime)
{
    std::for_each(data->children.begin(), data->children.end(), [deltaTime](std::shared_ptr<Object> object) {
        object->update(deltaTime);
    });
}

void Object::onUpdateObject(float deltaTime)
{
    (void)deltaTime;
}

ObjectVisitor::~ObjectVisitor()
{

}

void ObjectVisitor::visitObject(ObjectPointer object)
{
    (void)object;
}
