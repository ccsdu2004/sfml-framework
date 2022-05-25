#include <SFML/Graphics/Text.hpp>
#include <Application.h>
#include <Widget/Label.h>
#include <Widget/Desktop.h>
#include <Widget/WidgetStyle.h>

class LabelData
{
public:
    LabelData()
    {
        text = std::make_shared<sf::Text>();
    }

    std::shared_ptr<sf::Text> text;
    std::shared_ptr<LabelStyle> style;
};

Label::Label(const sf::Vector2f &size, const std::optional<CornerStyle> &cornerStyle):
    Widget(size, cornerStyle),
    data(new LabelData())
{
    auto stylePool = Application::getInstance()->getComponent<Desktop>()->getStylePointer();
    assert(stylePool);
    data->style = stylePool->getComponent<LabelStyle>();
    assert(data->style);

    setWidgetStyle(data->style);
}

Label::~Label()
{
}

void Label::setText(const sf::String &text)
{
    data->text->setString(text);
}

sf::String Label::getText() const
{
    return data->text->getString();
}

void Label::onPositionChanged()
{
    auto textBox = data->text->getGlobalBounds();
    auto position = Widget::adjustPosition(getBoundingBox(), sf::Vector2f(textBox.width,
                                           textBox.height),
                                           data->style->hMode,
                                           data->style->vMode, getPadding(), getPadding());

    auto box = getBoundingBox();
    data->text->setPosition(position.x - box.left,
                            position.y - box.top);
}

void Label::onStyleChanged()
{
    Widget::onStyleChanged();
    auto style = getWidgetStyle();
    if (style && std::dynamic_pointer_cast<LabelStyle>(style)) {
        data->style = std::dynamic_pointer_cast<LabelStyle>(style);
    }

    data->text->setFont(*Application::getInstance()->loadFont(data->style->font));
    data->text->setCharacterSize(data->style->size);
    data->text->setStyle(data->style->style);
    data->text->setFillColor(data->style->textColor);
    data->text->setPosition(getSize() * 0.5f);

    onPositionChanged();
}

void Label::onDrawObject(sf::RenderTarget &target, sf::RenderStates states) const
{
    Widget::onDrawObject(target, states);
    target.draw(*data->text.get(), states);
}
