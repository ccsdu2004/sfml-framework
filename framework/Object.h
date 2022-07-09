#pragma once
#include <memory>
#include <functional>
#include <SFML/Graphics/Drawable.hpp>
#include <Def.h>
#include <Message.h>

class ObjectVisitor;

enum ObjectRenderOrder {
    ObjectRenderOrder_ChildrenFirst,
    ObjectRenderOrder_ParentFirst
};

class Object : public sf::Drawable, public MessageReceiver,
    public std::enable_shared_from_this<Object>
{
public:
    Object();
    virtual ~Object();
public:
    void addChild(ObjectPointer child);
    void removeChild(ObjectPointer child);
    void removeChild(std::function<bool(ObjectPointer)> fn);
    void foreachChild(std::function<void(ObjectPointer)> fn);

    std::weak_ptr<Object> getParent()const;

    void acceptObject(ObjectVisitor *visitor);
public:
    virtual bool process(std::shared_ptr<Message> message)override;
    void update(float deltaTime);

    virtual bool needRemoved()const;

    void setObjectRenderOrder(ObjectRenderOrder order);
    ObjectRenderOrder getObjectRenderOrder()const;
    void setRenderObject(bool render = true);
    bool isRenderObject()const;
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
protected:
    virtual void onDrawObject(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void onUpdateChildren(float deltaTime);
    virtual void onUpdateObject(float deltaTime);
private:
    std::unique_ptr<class ObjectData> data;
};

class ObjectVisitor
{
public:
    virtual ~ObjectVisitor();
public:
    virtual void visitObject(ObjectPointer object);
};
