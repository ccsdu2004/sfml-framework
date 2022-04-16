#pragma once
#include <Entity.h>

class Switch : public Entity
{
public:
    Switch();
    virtual ~Switch();
public:
    void next();
    void prev();

    void clear();
    void addEntity(EntityPointer entity);

    uint32_t getEntityCount()const;
    void setCurrent(uint32_t index);
    uint32_t getCurrent()const;
protected:
    void onPositionChanged() override;
    void onDraw(sf::RenderTarget &target, sf::RenderStates states)const override;
private:
    std::unique_ptr<class SwitchData> data;
};
