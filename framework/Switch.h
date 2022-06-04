#pragma once
#include <cassert>
#include <Entity.h>
#include <Bitmask.h>

class Switch : public Entity
{
public:
    Switch();
    virtual ~Switch();
public:
    void next();
    void prev();

    void clear();
    void addItem(EntityPointer entity);

    int32_t getItemCount()const;
    void setCurrentItem(uint32_t index);
    int32_t getCurrentItem()const;
protected:
    virtual void onClear();
    void onDrawObject(sf::RenderTarget &target, sf::RenderStates states) const override;
private:
    std::unique_ptr<class SwitchData> data;
};

