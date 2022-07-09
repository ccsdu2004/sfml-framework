#include <map>
#include <Object.h>
#include <Entity.h>
#include <Sprite.h>
#include <Switch.h>
#include <Animation.h>
#include <iostream>

class ObjectData
{
public:
    std::list<ObjectPointer> children;
    std::weak_ptr<Object> parent;
    ObjectRenderOrder renderOrder = ObjectRenderOrder_ParentFirst;
    bool objectRender = true;

    void removeIfNecessary()
    {
        auto fn = [](ObjectPointer object)->bool{return object->needRemoved();};
        children.remove_if(fn);
    }
};

Object::Object():
    data(new ObjectData())
{
}

Object::~Object()
{
}

std::weak_ptr<Object> Object::getParent() const
{
    return data->parent;
}

void Object::addChild(ObjectPointer child)
{
    if (!child)
        return;

    auto weakParent = child->getParent();
    if (!weakParent.expired()) {
        auto childParent = weakParent.lock();
        if (childParent == shared_from_this())
            return;

        childParent->removeChild(child);
        return;
    }

    data->children.push_back(child);
    child->data->parent = weak_from_this();
}

void Object::removeChild(ObjectPointer child)
{
    if (!child)
        return;

    data->children.remove(child);
    child->data->parent.reset();
}

void Object::removeChild(std::function<bool (ObjectPointer)> fn)
{
    auto thisFn = [&fn](ObjectPointer object)->bool {
        bool ret = fn(object);
        if (ret)
        {
            object->data->parent.reset();
        }
        return ret;
    };

    data->children.remove_if(thisFn);
}

void Object::foreachChild(std::function<void (ObjectPointer)> fn)
{
    std::for_each(data->children.begin(), data->children.end(), fn);
}

void Object::acceptObject(ObjectVisitor *visitor)
{
    if (!visitor)
        return;

    std::for_each(data->children.begin(),
    data->children.end(), [visitor](std::shared_ptr<Object> object) {
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
    data->removeIfNecessary();
}

bool Object::needRemoved() const
{
    return false;
}

void Object::setObjectRenderOrder(ObjectRenderOrder order)
{
    data->renderOrder = order;
}

ObjectRenderOrder Object::getObjectRenderOrder() const
{
    return data->renderOrder;
}

void Object::setRenderObject(bool render)
{
    data->objectRender = render;
}

bool Object::isRenderObject() const
{
    return data->objectRender;
}
void Object::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    auto entity = dynamic_cast<const Entity *>(this);
    if (entity)
        states.transform *= entity->getTransform();

    if (data->renderOrder == ObjectRenderOrder_ParentFirst) {
        onDrawObject(target, states);

        std::for_each(data->children.begin(), data->children.end(), [&target,
        states](std::shared_ptr<Object> object) {
            object->draw(target, states);
        });
    } else {
        std::for_each(data->children.begin(), data->children.end(), [&target,
        states](std::shared_ptr<Object> object) {
            object->draw(target, states);
        });
        onDrawObject(target, states);
    }
}

void Object::onDrawObject(sf::RenderTarget &target, sf::RenderStates states) const
{
    (void)target;
    (void)states;
}

void Object::onUpdateChildren(float deltaTime)
{
    std::for_each(data->children.begin(),
    data->children.end(), [deltaTime](std::shared_ptr<Object> object) {
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
