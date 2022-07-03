#pragma once
#include <optional>
#include <boost/core/demangle.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <CornerStyle.h>
#include <Entity.h>

struct OutlineStyle {
    sf::Color color = sf::Color::White;
    float thickness = 1.0f;
};

class WidgetStyle
{
public:
    virtual std::string getClassName()const
    {
        return boost::core::demangle(typeid(*this).name());
    }

    sf::Color normalColor = sf::Color(128, 153, 128, 180);
    std::optional<sf::Color> disableColor = sf::Color(102, 102, 102, 140);
    std::optional<sf::Color> pressedColor = sf::Color(142, 102, 102, 140);
    std::optional<sf::Color> hoverColor;

    std::optional<OutlineStyle> outlineStyle;
    std::optional<OutlineStyle> outlinePressedStyle;

    sf::Color getDisableColor()const;
    sf::Color getHoverColor()const;
};

using WidgetStylePointer = std::shared_ptr<WidgetStyle>;

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
    sf::Color textColor = sf::Color::Black;
};

class BasicButtonStyle : public WidgetStyle
{
public:
    BasicButtonStyle()
    {
        hoverColor = sf::Color(153, 128, 128, 140);
    }

    virtual std::string getClassName()const
    {
        return boost::core::demangle(typeid(*this).name());
    }

    sf::Color pressedColor = sf::Color(153, 138, 138, 140);
};

class ButtonStyle : public BasicButtonStyle
{
public:
    ButtonStyle()
    {
        hoverColor = sf::Color(153, 128, 128, 140);
    }

    virtual std::string getClassName()const
    {
        return boost::core::demangle(typeid(*this).name());
    }

    sf::Color pressedColor = sf::Color(153, 138, 138, 140);
    std::string font = "../resource/FZYTK.TTF";
    float size = 18.0f;

    sf::Text::Style style = sf::Text::Regular;
    sf::Color textColor = sf::Color::White;
};

class ProgressBarStyle : public WidgetStyle
{
public:
    ProgressBarStyle()
    {
    }

    virtual std::string getClassName()const
    {
        return boost::core::demangle(typeid(*this).name());
    }

    sf::Color progressBarColor = sf::Color(180, 120, 120, 140);
    std::string font = "../resource/FZYTK.TTF";
    float size = 12.0f;

    sf::Text::Style style = sf::Text::Regular;
    sf::Color textColor = sf::Color::White;
};

class ImageBoxStyle : public WidgetStyle
{
public:
    virtual std::string getClassName()const
    {
        return boost::core::demangle(typeid(*this).name());
    }
};

class PanelStyle : public WidgetStyle
{
public:
    PanelStyle()
    {
        titleStyle = std::make_shared<LabelStyle>();
        titleStyle->textColor = sf::Color::White;
    }

    std::shared_ptr<LabelStyle> titleStyle;

    virtual std::string getClassName()const
    {
        return boost::core::demangle(typeid(*this).name());
    }
};
