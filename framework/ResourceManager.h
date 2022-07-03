#pragma once
#include <cassert>
#include <string>
#include <map>
#include <memory>
#include <Component.h>

struct ResourceDummy {
};

template<class T>
class ResourceManager : public Component
{
public:
    ResourceManager() = default;

    void update(float deltaTime) override
    {
        (void)deltaTime;
    }

    std::shared_ptr<T> loadFromFile(const std::string& file)
    {
        auto itr = resources.find(file);
        if(itr != resources.end())
            return itr->second;

        auto resource = std::make_shared<T>();
        if (!resource->loadFromFile(file))
            return nullptr;

        resources.insert(std::make_pair(file, resource));
        return resource;
    }

    template<class Arg = ResourceDummy>
    std::shared_ptr<T> loadFromFile(const std::string& file, const std::string& name, const Arg& arg = Arg())
    {
        auto itr = resources.find(name);
        if(itr != resources.end())
            return itr->second;

        std::shared_ptr<T> resource = std::make_shared<T>();
        if(std::is_same<Arg, ResourceDummy>::value) {
            if (!resource->loadFromFile(file)) {
                return nullptr;
            }
        } else {
            if (!resource->loadFromFile(file, arg)) {
                return nullptr;
            }
        }

        resources.insert(std::make_pair(name, resource));
        return resource;
    }

    std::shared_ptr<T> getResource(const std::string& name)
    {
        auto itr = resources.find(name);
        return itr != resources.end() ? *itr : nullptr;
    }

    void removeResource(const std::string& file)
    {
        resources.erase(file);
    }
private:
    std::map<std::string, std::shared_ptr<T>> resources;
};
