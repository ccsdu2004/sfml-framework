#include <Widget/WidgetLayout.h>

class WidgetLayoutData
{
public:
    std::vector<std::pair<std::shared_ptr<Widget>, std::shared_ptr<WidgetLayoutInfo>>> widgets;
    float margin = 4.0f;
    float spacing = 3.0f;

    bool dirty = true;
};

WidgetLayout::WidgetLayout():
    data(new WidgetLayoutData())
{
}

WidgetLayout::~WidgetLayout()
{
}

void WidgetLayout::setMargin(float margin)
{
    if(data->margin != margin) {
        data->margin = margin;
        adjust();
    }
}

float WidgetLayout::getMargin()const
{
    return data->margin;
}

void WidgetLayout::setSpacing(float spacing)
{
    if(data->spacing != spacing) {
        data->spacing = spacing;
        adjust();
    }
}

float WidgetLayout::getSpacing()const
{
    return data->spacing;
}

void WidgetLayout::addWidget(WidgetPointer widget, std::shared_ptr<WidgetLayoutInfo> info)
{
    data->widgets.push_back(std::make_pair(widget, info));
    data->dirty = true;
}

uint32_t WidgetLayout::getWidgetCount() const
{
    return data->widgets.size();
}

std::pair<WidgetPointer, std::shared_ptr<WidgetLayoutInfo>> WidgetLayout::getWidgetByIndex(uint32_t index)
{
    if(index >= getWidgetCount())
        return std::pair<WidgetPointer, std::shared_ptr<WidgetLayoutInfo>>(nullptr, nullptr);
    return data->widgets.at(index);
}

WidgetPointer WidgetLayout::getWidgetBelow(const sf::Vector2i &point)
{
    auto itr = data->widgets.begin();
    while(itr != data->widgets.end()) {
        if((*itr).first->getWidgetBelow(point))
            return itr->first;
        itr ++;
    }
    return Widget::getWidgetBelow(point);
}

bool WidgetLayout::doesHierarchyContain(WidgetPointer other)const
{
    auto itr = data->widgets.begin();
    while(itr != data->widgets.end()) {
        if(itr->first->doesHierarchyContain(other))
            return true;
        itr ++;
    }
    return Widget::doesHierarchyContain(other);
}

bool WidgetLayout::process(std::shared_ptr<Message> message)
{
    auto itr = data->widgets.begin();
    while(itr != data->widgets.end()) {
        if(itr->first->process(message))
            return true;
        itr ++;
    }
    return Widget::process(message);
}

void WidgetLayout::onActiveChanged()
{

}

void WidgetLayout::onVisibleChanged()
{

}

void WidgetLayout::onGainFocus()
{

}

void WidgetLayout::onLostFocus()
{

}

void WidgetLayout::onMovableChanged()
{

}

void WidgetLayout::onPositionChanged()
{
    adjust();
}

void WidgetLayout::onSizeChanged()
{

}

void WidgetLayout::onUpdateObject(float deltaTime)
{
    if(data->dirty) {
        adjust();
        data->dirty = false;
    }

    auto itr = data->widgets.begin();
    while(itr != data->widgets.end()) {
        auto widget = itr->first;
        widget->update(deltaTime);
        itr ++;
    }
}

void WidgetLayout::onDrawObject(sf::RenderTarget &target, sf::RenderStates states) const
{
    Entity::onDrawObject(target, states);
    auto itr = data->widgets.begin();
    while(itr != data->widgets.end()) {
        auto widget = itr->first;
        widget->draw(target, states);
        itr ++;
    }
}
