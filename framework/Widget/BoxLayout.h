#pragma once
#include <Widget/WidgetLayout.h>

class VBoxLayout : public WidgetLayout
{
public:
    VBoxLayout();
    ~VBoxLayout();
public:
    void addWidget(WidgetPointer widget, HMode mode = HMode_Center);
protected:
    void adjust()override;
private:
    std::unique_ptr<class VBoxLayoutData> data;
};

class HBoxLayout : public WidgetLayout
{
public:
    HBoxLayout();
    ~HBoxLayout();
public:
    void addWidget(WidgetPointer widget, VMode mode = VMode_Center);
protected:
    void adjust()override;
private:
    std::unique_ptr<class HBoxLayoutData> data;
};
