#pragma once
#include <memory>
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

    virtual void accept(ObjectVisitor *visitor);

    bool process(std::shared_ptr<Message> message)override;
    virtual void update(float deltaTime);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
protected:
    virtual void onDrawObject(sf::RenderTarget &target, sf::RenderStates states) const;
private:
    std::unique_ptr<class ObjectData> data;
};

class ObjectVisitor
{
public:
    virtual ~ObjectVisitor();
public:
    template<class Type>
    void visitObject(ObjectPointer object)
    {
        auto item = std::dynamic_pointer_cast<Type>(object);
        if (item)
            visit(item);
    }

    virtual void visit(ObjectPointer object);
    virtual void visit(EntityPointer entity);
    virtual void visit(SpritePointer sprite);
    virtual void visit(SwitchPointer switchObject);
    virtual void visit(AnimationPointer animation);
};
