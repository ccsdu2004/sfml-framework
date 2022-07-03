#pragma once
#include <memory>

class WidgetSelector
{
public:
    WidgetSelector();
    virtual ~WidgetSelector();
public:
    void setRadioMode(bool radioMode = true);
    bool isRadioMode()const;

    virtual void select() = 0;
    virtual void unselect() = 0;
private:
    std::unique_ptr<class WidgetSelectorData> data;
};
