#include <Application.h>
#include <Widget/Widget.h>

class WidgetData
{
public:
    std::weak_ptr<Widget> parent;
    bool active = true;
    bool visible = true;
    bool focus = false;
    float padding = 3.0f;
    RangeFloat widthRange = {0.0f, 3000.0f};
    RangeFloat heightRange = {0.0f, 3000.0f};
};

Widget::Widget(const sf::Vector2f &size,
               const std::optional<CornerStyle> &cornerStyle):
    Entity(size, cornerStyle),
    data(new WidgetData())
{
}

Widget::~Widget()
{

}

void Widget::setPadding(float padding)
{
    data->padding = padding;
}

bool Widget::getPadding() const
{
    return data->padding;
}

void Widget::setWidth(float width)
{
    setSize(width, getHeight());
}

float Widget::getWidth() const
{
    return getSize().x;
}

void Widget::setHeight(float height)
{
    setSize(getWidth(), height);
}

float Widget::getHeight() const
{
    return getSize().y;
}

void Widget::setParent(std::weak_ptr<Widget> parent)
{
    data->parent = parent;
}

std::weak_ptr<Widget> Widget::getParent() const
{
    return data->parent;
}

void Widget::setWidthRange(const RangeFloat &range)
{
    if (!range.isValid())
        return;

    data->widthRange = range;
    float width = clamp(getWidth(), range);
    setWidth(width);
}

RangeFloat Widget::getWidthRange() const
{
    return data->widthRange;
}

void Widget::setHeightRange(const RangeFloat &range)
{
    if (!range.isValid())
        return;

    data->heightRange = range;
    float height = clamp(getHeight(), range);
    setHeight(height);
}

RangeFloat Widget::getHeightRange() const
{
    return data->heightRange;
}

void Widget::setActive(bool active)
{
    if (isActive() == active)
        return;
    data->active = active;
    onActiveChanged();
}

bool Widget::isActive() const
{
    return data->active;
}

void Widget::setVisible(bool visible)
{
    if (isVisible() == visible)
        return;
    data->visible = visible;
    onVisibleChanged();
}

void Widget::show()
{
    if (isVisible())
        return;
    setVisible(true);
}

void Widget::hide()
{
    if (!isVisible())
        return;
    setVisible(false);
}

bool Widget::isVisible() const
{
    return data->visible;
}

void Widget::setFocused(bool focus)
{
    if (isFocused() == focus)
        return;
    data->focus = focus;
    if (focus)
        onGainFocus();
    else
        onLostFocus();
}

bool Widget::isFocused() const
{
    return data->focus;
}

bool Widget::isUnderMouse()
{
    auto mousePosition = sf::Mouse::getPosition(*Application::getInstance()->getWindow());
    return getBoundingBox().contains(mousePosition.x, mousePosition.y);
}

bool Widget::containsPoint(const sf::Vector2i &point)
{
    return getBoundingBox().contains(point.x, point.y);
}

WidgetPointer Widget::getWidgetBelow(const sf::Vector2i &point)
{
    if (isVisible() && containsPoint(point))
        return std::dynamic_pointer_cast<Widget>(shared_from_this());
    return nullptr;
}

bool Widget::doesHierarchyContain(WidgetPointer other) const
{
    return this == other.get();
}

void Widget::onActiveChanged()
{

}

void Widget::onVisibleChanged()
{

}

void Widget::onGainFocus()
{

}

void Widget::onLostFocus()
{

}

void Widget::onPositionChanged()
{

}

void Widget::onSizeChanged()
{
    auto widthRange = getWidthRange();
    setWidthRange(widthRange);

    auto heightRange = getHeightRange();
    setHeightRange(heightRange);
}

void Widget::onUpdate(float deltaTime)
{

}

void Widget::onDrawObject(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (!isVisible())
        return;

    Entity::onDrawObject(target, states);
}
