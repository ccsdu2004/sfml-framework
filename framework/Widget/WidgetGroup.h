#pragma once
#include <Widget/Widget.h>

class WidgetGroup
{
public:
    WidgetGroup();
    virtual ~WidgetGroup();
public:
    void addWidget(WidgetPointer widget);

    virtual void onSelected(WidgetPointer widget);
    virtual void onUnselected(WidgetPointer widget);
private:
    std::unique_ptr<class WidgetGroupData> data;
};
