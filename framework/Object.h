#pragma once
#include <memory>
#include <functional>
#include <SFML/Graphics/Drawable.hpp>
#include <Def.h>
#include <Message.h>

class ObjectVisitor;

class Object : public sf::Drawable, public MessageReceiver,
    public std::enable_shared_from_this<Object>
{
public:
    Object();
    virtual ~Object();
public:
    void addChild(std::shared_ptr<Object> child);
    void removeChild(std::shared_ptr<Object> child);
    void removeChild(std::function<bool(std::shared_ptr<Object>)> fn);

    void acceptObject(ObjectVisitor *visitor);

    bool process(std::shared_ptr<Message> message)override;
    void update(float deltaTime);

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
protected:
    virtual void onDrawObject(sf::RenderTarget &target, sf::RenderStates states) const;
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
