#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>
#include <Switch.h>
#include <iostream>

class SwitchData
{
public:
    std::vector<EntityPointer> list;
    uint32_t index = 0;
};

Switch::Switch():
    data(new SwitchData())
{
}

Switch::~Switch()
{
}

void Switch::next()
{
    data->index ++;
}

void Switch::prev()
{
    data->index --;
}

void Switch::clear()
{
    data->list.clear();
    data->index = ~0;
    onClear();
}

void Switch::addItem(const EntityPointer entity)
{
    if (entity) {
        data->list.push_back(entity);
    }
}

uint32_t Switch::getItemCount()const
{
    return data->list.size();
}

void Switch::setCurrentItem(uint32_t index)
{
    data->index = index;
}

uint32_t Switch::getCurrentItem() const
{
    return data->index;
}

void Switch::onClear()
{
}

void Switch::onDrawObject(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (data->list.size() <= data->index)
        return;

    auto current = data->list[data->index];
    data->list[data->index]->draw(target, states);
}




