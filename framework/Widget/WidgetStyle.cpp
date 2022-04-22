#include <Widget/WidgetStyle.h>

sf::Color WidgetStyle::getDisableColor() const
{
    if (disableColor.has_value())
        return disableColor.value();
    return normalColor;
}

sf::Color WidgetStyle::getHoverColor() const
{
    if (hoverColor.has_value())
        return hoverColor.value();
    return normalColor;
}

sf::Color WidgetStyle::getActiveColor() const
{
    if (activeColor.has_value())
        return activeColor.value();
    return normalColor;
}
