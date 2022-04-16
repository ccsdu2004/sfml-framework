#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>
#include <Switch.h>

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
}

void Switch::addEntity(const std::shared_ptr<Entity> entity)
{
    if(entity) {
        auto pos = getPosition();
        entity->setPosition(pos.x, pos.y);
        data->list.push_back(entity);
    }
}

uint32_t Switch::getEntityCount()const
{
    return data->list.size();
}

void Switch::setCurrent(uint32_t index)
{
    data->index = index;
}

uint32_t Switch::getCurrent() const
{
    return data->index;
}

void Switch::onPositionChanged()
{
    auto pos = getPosition();
    auto itr = data->list.begin();
    while(itr != data->list.end()) {
        auto entity = *itr;
        entity->setPosition(pos.x, pos.y);
        itr ++;
    }
}

void Switch::onDraw(sf::RenderTarget &target, sf::RenderStates states)const
{
    if(data->list.size() <= data->index)
        return;

    data->list[data->index]->draw(target, states);
}
