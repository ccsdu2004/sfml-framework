#include "MouseArea.h"
#include "Application.h"
#include <iostream>

class MouseAreaData
{
public:
    bool movable = true;
    sf::Vector2i oldMousePosition;
    std::weak_ptr<Widget> target;
};

MouseArea::MouseArea(const sf::Vector2f &size, const std::optional<CornerStyle> &cornerStyle):
    Widget(size, cornerStyle),
    data(new MouseAreaData)
{
}

MouseArea::~MouseArea()
{
}

void MouseArea::setTargetWidget(std::shared_ptr<Widget> target)
{
    data->target = target;
}

std::shared_ptr<Widget> MouseArea::getTargetWidget() const
{
    return data->target.expired() ? nullptr : data->target.lock();
}

void MouseArea::onMouseEnter()
{
    data->oldMousePosition = sf::Mouse::getPosition(
                                 *Application::getInstance()->getWindow());
}

void MouseArea::onMouseExit()
{
}

void MouseArea::onMousePressed(sf::Mouse::Button button)
{
    data->oldMousePosition = sf::Mouse::getPosition(
                                 *Application::getInstance()->getWindow());
}

void MouseArea::onMouseReleased(sf::Mouse::Button button)
{

}

void MouseArea::onMouseMoved(int x, int y)
{
    if (data->target.expired())
        return;

    auto widget = data->target.lock();

    if (data->oldMousePosition != sf::Vector2i(x, y) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        int dx = x - data->oldMousePosition.x;
        int dy = y - data->oldMousePosition.y;
        widget->move(dx, dy);
        data->oldMousePosition.x = x;
        data->oldMousePosition.y = y;
    }
}

