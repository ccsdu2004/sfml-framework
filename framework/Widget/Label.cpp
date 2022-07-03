#include <SFML/Graphics/Text.hpp>
#include <Application.h>
#include <ResourceManager.h>
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
};

Label::Label(const sf::Vector2f &size, const std::optional<CornerStyle> &cornerStyle):
    Widget(size, cornerStyle),
    data(new LabelData())
{
    auto style = Application::getInstance()->getComponent<Desktop>()->getWidgetStyle("LabelStyle");
    assert(style);
    setWidgetStyle(style);
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

void Label::onDrawWidget(sf::RenderTarget &target, sf::RenderStates states) const
{
    Widget::onDrawWidget(target, states);
    target.draw(*data->text.get(), states);
}

void Label::onPositionChanged()
{
    auto style = std::dynamic_pointer_cast<LabelStyle>(getWidgetStyle());
    auto textBox = data->text->getGlobalBounds();
    auto position = Widget::adjustPosition(getBoundingBox(), sf::Vector2f(textBox.width,
                                           textBox.height),
                                           style->hMode,
                                           style->vMode, getPadding(), getPadding());

    auto box = getBoundingBox();
    data->text->setPosition(position.x - box.left,
                            position.y - box.top);
}

void Label::onStyleChanged()
{
    Widget::onStyleChanged();

    auto style = std::dynamic_pointer_cast<LabelStyle>(getWidgetStyle());

    auto fontManager = Application::getInstance()->getComponent<ResourceManager<sf::Font>>();
    auto font = fontManager->loadFromFile(style->font);

    data->text->setFont(*font);
    data->text->setCharacterSize(style->size);
    data->text->setStyle(style->style);
    data->text->setFillColor(style->textColor);
    data->text->setPosition(getSize() * 0.5f);

    onPositionChanged();
}

