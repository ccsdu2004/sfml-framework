#pragma once
#include <map>
#include <functional>

#define STR(str) #str
#define A_B(a,b) a##b

template<class Product, class ID>
class ObjectFactory
{
public:
    static Product *create(const ID &id)
    {
        auto itr = creator->find(id);
        if (itr != creator->end())
            return itr->second();
        return nullptr;
    }

    static std::map<ID, std::function<Product*()>> *creator;
    static void initialize()
    {
        if (!creator)
            creator = new std::map<ID, std::function<Product*()>>();
    }

    static void shutdown()
    {
        if (creator)
            delete creator;
        creator = nullptr;
    }
};

template<class Product, class Target, class ID>
class ObjectRegister : public ObjectFactory<Product, ID>
{
public:
    ObjectRegister(const ID &id)
    {
        ObjectFactory<Product, ID>::initialize();

        (*ObjectFactory<Product, ID>::creator)[id] = []()->Product * {
            return new Target();
        };
    }
};

template<class Product, class ID>
std::map<ID, std::function<Product*()>> *ObjectFactory<Product, ID>::creator = nullptr;

#define REGISTER_OBJECT(base,product)\
    ObjectRegister<base,product,std::string> A_B(base,product)(STR(product));
