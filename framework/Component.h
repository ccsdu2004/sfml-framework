#pragma once
#include <list>
#include <memory>
#include <type_traits>
#include <iostream>
#include <Updater.h>

class Component : public Updater
{
public:
    virtual ~Component()
    {
    }
};

using ComponentPointer = std::shared_ptr<Component>;

class ComponentVisitor
{
public:
    virtual ~ComponentVisitor() {}
    virtual void visitComponent(ComponentPointer component) = 0;
};

class ComponentPool
{
public:
    virtual ~ComponentPool() {}
public:
    template<class T>
    void addComponent(std::shared_ptr<T> pointer)
    {
        auto component = getComponent<T>();
        if (component)
            return;

        if(!pointer)
            return;

        static_assert (std::is_base_of<Component, T>() && "component must be derived of Component");
        components.push_back(pointer);
    }

    template<class T>
    std::shared_ptr<T> getComponent()const
    {
        auto  itr = components.begin();
        while (itr != components.end()) {
            auto ptr = std::dynamic_pointer_cast<T>(*itr);
            if (ptr)
                return ptr;
            itr ++;
        }
        return nullptr;
    }

    void acceptComponent(ComponentVisitor *visitor)
    {
        if (!visitor)
            return;

        auto itr = components.begin();
        while (itr != components.end()) {
            visitor->visitComponent(*itr);
            itr ++;
        }
    }

    void updateComponent(float deltaTime)
    {
        auto itr = components.begin();
        while (itr != components.end()) {
            (*itr)->update(deltaTime);
            itr ++;
        }
    }
private:
    std::list<ComponentPointer> components;
};

using ComponentPoolPointer = std::shared_ptr<ComponentPool>;
