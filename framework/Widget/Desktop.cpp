#include <Application.h>
#include <Widget/Desktop.h>

class DesktopData
{
public:
    DesktopData()
    {
        stylePointer = std::make_shared<ComponentPool>();
        stylePointer->addComponent(std::make_shared<LabelStyle>());
        stylePointer->addComponent(std::make_shared<BasicButtonStyle>());
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

void Desktop::addWidget(WidgetPointer widget, float x, float y)
{
    if (!widget)
        return;

    widget->setPosition(x, y);
    data->widgets.push_back(widget);
}

void Desktop::addWidget(WidgetPointer widget, HMode hmode, VMode vmode, float xoffset,
                        float yoffset)
{
    if (!widget)
        return;

    auto windowSize = Application::getInstance()->getWindow()->getSize();

    auto size = widget->getSize();
    sf::FloatRect area(0, 0, windowSize.x, windowSize.y);
    auto position = Widget::adjustPosition(area, size, hmode, vmode, xoffset, yoffset);
    widget->setPosition(position + size * 0.5f);
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

void Desktop::update(float deltaTime)
{
    std::for_each(data->widgets.begin(),
    data->widgets.end(), [deltaTime](std::shared_ptr<Widget> widget) {
        widget->update(deltaTime);
    });
}

void Desktop::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    auto itr = data->widgets.begin();
    while (itr != data->widgets.end()) {
        (*itr)->draw(target, states);
        itr ++;
    }

    if (data->popupWidget)
        data->popupWidget->draw(target, states);
}

