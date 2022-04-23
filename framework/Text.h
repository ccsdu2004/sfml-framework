#pragma once
#include <SFML/Graphics/Text.hpp>
#include <Entity.h>

class Text : public Entity
{
public:
    Text(const sf::Vector2f &size = sf::Vector2f(96, 42));
    virtual ~Text();
public:
    void setCharacterSize(int pixelSize);
    void setTextColor(const sf::Color &color);

    void setText(const sf::String &text, bool resize = true);
    sf::String getText()const;

    void setStyle(sf::Text::Style style);

    void setHMode(HMode mode);
    HMode getHMode()const;

    void setVMode(VMode mode);
    VMode getVMode()const;

    void setFont(std::shared_ptr<sf::Font> font);
public:
    void onPositionChanged()override;
    void onSizeChanged()override;
    void onDrawObject(sf::RenderTarget &target, sf::RenderStates states) const override;
private:
    std::unique_ptr<class TextData> data;
};

