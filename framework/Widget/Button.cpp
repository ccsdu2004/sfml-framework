#include <Application.h>
#include <Widget/Desktop.h>
#include <Widget/Button.h>

class ButtonData
{
public:
    ButtonData()
    {
        text = std::make_shared<sf::Text>();
    }

    std::shared_ptr<sf::Text> text;
};

Button::Button(const sf::Vector2f &size, const std::optional<CornerStyle> &cornerStyle):
    BasicButton(size, cornerStyle),
    data(new ButtonData())
{
    auto style = Application::getInstance()->getComponent<Desktop>()->getWidgetStyle("ButtonStyle");
    assert(style);
    setWidgetStyle(style);
}

Button::~Button()
{

}

void Button::setText(const sf::String &text)
{
    if (text != data->text->getString()) {
        data->text->setString(text);
        onPositionChanged();
    }
}

sf::String Button::getText() const
{
    return data->text->getString();
}

void Button::onDrawWidget(sf::RenderTarget &target, sf::RenderStates states) const
{
    BasicButton::onDrawWidget(target, states);
    target.draw(*data->text.get(), states);
}

void Button::onPositionChanged()
{
    auto style = std::dynamic_pointer_cast<ButtonStyle>(getWidgetStyle());
    auto textBox = data->text->getGlobalBounds();
    auto position = Widget::adjustPosition(getBoundingBox(), sf::Vector2f(textBox.width,
                                                                          textBox.height),
                                           HMode_Center,
                                           VMode_Center, getPadding(), getPadding());

    auto box = getBoundingBox();
    data->text->setPosition(position.x - box.left,
                            position.y - box.top);
}

void Button::onStyleChanged()
{
    BasicButton::onStyleChanged();

    auto style = std::dynamic_pointer_cast<ButtonStyle>(getWidgetStyle());

    data->text->setFont(*Application::getInstance()->loadFont(style->font));
    data->text->setCharacterSize(style->size);
    data->text->setStyle(style->style);
    data->text->setFillColor(style->textColor);
    data->text->setPosition(getSize() * 0.5f);

    onPositionChanged();
}

void Button::onButtonStateChanged()
{
    auto state = getButtonState();
    if (state == BasicButtonState_Down)
        recalcTextPosition(1.0f, 1.0f);
    else
        recalcTextPosition(0.0f, 0.0f);
}

void Button::recalcTextPosition(float offsetx, float offsety)
{
    auto style = std::dynamic_pointer_cast<ButtonStyle>(getWidgetStyle());
    auto textBox = data->text->getGlobalBounds();
    auto box = getBoundingBox();
    auto position = Widget::adjustPosition(box, sf::Vector2f(textBox.width,
                                                             textBox.height),
                                           HMode_Center,
                                           VMode_Center, getPadding(), getPadding());

    data->text->setPosition(position.x - box.left + offsetx,
                            position.y - box.top + offsety);
}
