#include <Widget/WidgetSelector.h>

class WidgetSelectorData
{
public:
    bool radioMode = true;
};

WidgetSelector::WidgetSelector():
    data(new WidgetSelectorData())
{
}

WidgetSelector::~WidgetSelector()
{

}

void WidgetSelector::setRadioMode(bool radioMode)
{
    data->radioMode = radioMode;
}

bool WidgetSelector::isRadioMode() const
{
    return data->radioMode;
}
