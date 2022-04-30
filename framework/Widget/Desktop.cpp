#include <Application.h>
#include <Widget/Desktop.h>

class DesktopData
{
public:
    DesktopData()
    {
        stylePointer = std::make_shared<ComponentPool>();
    }

    std::list<WidgetPointer> widgets;
    WidgetPointer focusedWidget;
    WidgetPointer popupWidget;
    WidgetPointer belowCursorWidget;

    ComponentPoolPointer stylePointer;
};

Desktop::Desktop():
    data(new DesktopData())
{
}

Desktop::~Desktop()
{
}

void Desktop::addWidget(WidgetPointer widget, int x, int y)
{
    if (!widget)
        return;

    widget->setPosition(x, y);
    data->widgets.push_back(widget);
}

void Desktop::removeWidget(WidgetPointer widget)
{
    if (!widget)
        return;

    data->widgets.remove_if([&widget](WidgetPointer input) {
        return widget == input;
    });

    //check focused and ...
}

void Desktop::clearAllWidgets()
{
    data->widgets.clear();
    data->focusedWidget = nullptr;
}

WidgetPointer Desktop::getFocusWidget() const
{
    return data->focusedWidget;
}

void Desktop::setFocusWidget(WidgetPointer widget)
{
    if (!widget)
        return;

    if (widget != data->focusedWidget) {
        if (data->focusedWidget)
            data->focusedWidget->setFocused(false);
        data->focusedWidget = widget;
        data->focusedWidget->setFocused(true);
    }
}

WidgetPointer Desktop::getPopupWidget()const
{
    return data->popupWidget;
}

void Desktop::setPopupWidget(WidgetPointer widget)
{
    if (!widget)
        return;

    if (widget != data->popupWidget) {
        data->popupWidget = widget;
    }
}

WidgetPointer Desktop::widgetBelowCursor() const
{
    return data->belowCursorWidget;
}

bool Desktop::isTopWidget(WidgetPointer widget) const
{
    return widget ?
           std::find(data->widgets.begin(), data->widgets.end(), widget) != data->widgets.end()
           : false;
}

ComponentPoolPointer Desktop::getStylePointer() const
{
    return data->stylePointer;
}

bool Desktop::process(std::shared_ptr<Message> message)
{
    auto itr = data->widgets.begin();
    while (itr != data->widgets.end()) {
        if ((*itr)->process(message))
            return true;
        itr ++;
    }
    return MessageReceiver::process(message);
}

void Desktop::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    auto itr = data->widgets.begin();
    while (itr != data->widgets.end()) {
        (*itr)->draw(target, states);
        itr ++;
    }

    if(data->popupWidget)
        data->popupWidget->draw(target,states);
}
