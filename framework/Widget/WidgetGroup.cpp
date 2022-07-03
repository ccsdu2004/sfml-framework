#include <Widget/WidgetGroup.h>
#include <Widget/WidgetSelector.h>
#include <Widget/ImageBox.h>
#include <iostream>

class WidgetGroupData
{
public:
    std::list<std::weak_ptr<Widget>> widgets;
};

WidgetGroup::WidgetGroup():
    data(new WidgetGroupData())
{
}

WidgetGroup::~WidgetGroup()
{
}

void WidgetGroup::addWidget(WidgetPointer widget)
{
    if(widget)
        data->widgets.push_back(widget);
}

void WidgetGroup::onSelected(WidgetPointer widget)
{
    if(widget) {
        auto itr = data->widgets.begin();
        while(itr != data->widgets.end()) {
            auto current = itr->lock();
            if(current != widget) {
                auto imageBox = std::dynamic_pointer_cast<ImageBox>(current);
                if(imageBox) {
                    auto widgetSelect = std::dynamic_pointer_cast<WidgetSelector>(imageBox);
                    widgetSelect->unselect();
                }
            }
            itr ++;
        }
    }
}

void WidgetGroup::onUnselected(WidgetPointer widget)
{

}
