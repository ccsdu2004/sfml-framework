#include <SFML/Graphics/RenderTarget.hpp>
#include <Text.h>

class TextData
{
public:
    HMode hMode = HMode_Center;
    VMode vMode = VMode_Center;
    std::shared_ptr<sf::Text> text;
};

Text::Text(const sf::Vector2f &size):
    Entity(size),
    data(new TextData)
{
    data->text = std::make_shared<sf::Text>();
    data->text->setCharacterSize(18);
}

Text::~Text()
{
}

void Text::setCharacterSize(int pixelSize)
{
    data->text->setCharacterSize(pixelSize);
    onSizeChanged();
}

void Text::setTextColor(const sf::Color &color)
{
    data->text->setFillColor(color);
}

void Text::setText(const sf::String &text, bool resize)
{
    data->text->setString(text);
    if (resize) {
        auto box = data->text->getLocalBounds();
        setSize(box.width + 6, box.height + 6);
    }
    onPositionChanged();
    onSizeChanged();
}

sf::String Text::getText() const
{
    return data->text->getString();
}

void Text::setStyle(sf::Text::Style style)
{
    data->text->setStyle(style);
    onSizeChanged();
}

void Text::setHMode(HMode mode)
{
    data->hMode = mode;
    onSizeChanged();
}

HMode Text::getHMode() const
{
    return data->hMode;
}

void Text::setVMode(VMode mode)
{
    data->vMode = mode;
    onSizeChanged();
}

VMode Text::getVMode() const
{
    return data->vMode;
}

void Text::setFont(std::shared_ptr<sf::Font> font)
{
    if (font) {
        data->text->setFont(*font);
        onSizeChanged();
    }
}

void Text::onPositionChanged()
{
    auto box = data->text->getLocalBounds();
    auto newPosition = adjustPosition(getBoundingBox(), sf::Vector2f(box.width, box.height),
                                      data->hMode, data->vMode);
    data->text->setPosition(newPosition);
}

void Text::onSizeChanged()
{
    onPositionChanged();
}

void Text::onDrawObject(sf::RenderTarget &target, sf::RenderStates states) const
{
    Entity::onDrawObject(target, states);
    target.draw(*data->text);
}
