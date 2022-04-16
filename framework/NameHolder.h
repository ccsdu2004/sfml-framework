#pragma once
#include <string>

class NameHolder
{
public:
    virtual ~NameHolder();
public:
    void setName(const std::string& newName);
    std::string getName()const;
private:
    virtual void onNameChanged();
private:
    std::string name;
};
