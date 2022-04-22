#pragma once
#include <optional>
#include <SFML/Graphics/Color.hpp>
#include <CornerStyle.h>

struct OutlineStyle {
    sf::Color color = sf::Color::White;
    float thickness = 1.0f;
};

class WidgetStyle
{
public:
    sf::Color normalColor = sf::Color(128, 128, 128, 168);
    std::optional<sf::Color> disableColor;
    std::optional<sf::Color> hoverColor;
    std::optional<sf::Color> activeColor;

    std::optional<OutlineStyle> outlineStyle;

    sf::Color getDisableColor()const;
    sf::Color getHoverColor()const;
    sf::Color getActiveColor()const;
};
