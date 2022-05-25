#pragma once
#include <optional>
#include <boost/core/demangle.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <Component.h>
#include <CornerStyle.h>
#include <Entity.h>

struct OutlineStyle {
    sf::Color color = sf::Color::White;
    float thickness = 1.0f;
};

class WidgetStyle : public Component
{
public:
    void update(float deltaTime) override
    {
        (void)deltaTime;
    }

    virtual std::string getClassName()const
    {
        return boost::core::demangle(typeid(*this).name());
    }

    sf::Color normalColor = sf::Color(128, 153, 128, 255);
    std::optional<sf::Color> disableColor = sf::Color(102, 102, 102, 140);
    std::optional<sf::Color> hoverColor = sf::Color(153, 128, 128, 140);
    std::optional<sf::Color> activeColor = sf::Color(128, 108, 128, 168);

    std::optional<OutlineStyle> outlineStyle;

    sf::Color getDisableColor()const;
    sf::Color getHoverColor()const;
    sf::Color getActiveColor()const;
};

class LabelStyle : public WidgetStyle
{
public:
    virtual std::string getClassName()const
    {
        return boost::core::demangle(typeid(*this).name());
    }

    std::string font = "../resource/FZYTK.TTF";
    float size = 15.0f;
    HMode hMode = HMode_Center;
    VMode vMode = VMode_Center;

    sf::Text::Style style;
    sf::Color textColor;
};

class BasicButtonStyle : public WidgetStyle
{
public:
    virtual std::string getClassName()const
    {
        return boost::core::demangle(typeid(*this).name());
    }

    sf::Color pressedColor = sf::Color(153, 138, 138, 140);
};
