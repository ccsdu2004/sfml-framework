#include <NameHolder.h>

NameHolder::~NameHolder()
{
}

void NameHolder::setName(const std::string &newName)
{
    if (name != newName) {
        name = newName;
        onNameChanged();
    }
}

std::string NameHolder::getName() const
{
    return name;
}

void NameHolder::onNameChanged()
{
}
