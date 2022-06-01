#include <Application.h>
#include <Widget/Desktop.h>
#include <Widget/BasicButton.h>

class BasicButtonData
{
public:
    BasicButtonState buttonState = BasicButtonState::BasicButtonState_Normal;
};

BasicButton::BasicButton(const sf::Vector2f &size, const std::optional<CornerStyle> &cornerStyle):
    Widget(size, cornerStyle),
    data(new BasicButtonData)
{
    auto style =
        Application::getInstance()->getComponent<Desktop>()->getWidgetStyle("BasicButtonStyle");
    assert(style);
    setWidgetStyle(style);
}

BasicButton::~BasicButton()
{

}

BasicButtonState BasicButton::getButtonState()const
{
    return data->buttonState;
}

void BasicButton::onMouseEnter()
{
    if (isActive()) {
        data->buttonState = BasicButtonState_Hover;
        onButtonStateChanged();

        auto style = std::dynamic_pointer_cast<BasicButtonStyle>(getWidgetStyle());
        if (!style)
            return;

        setBackgroundColor(style->getHoverColor());
    }

    Widget::onMouseEnter();
}

void BasicButton::onMouseExit()
{
    if (isActive()) {
        data->buttonState = BasicButtonState_Normal;

        auto style = std::dynamic_pointer_cast<BasicButtonStyle>(getWidgetStyle());
        if (!style)
            return;

        setBackgroundColor(style->normalColor);
    }

    Widget::onMouseExit();
}

void BasicButton::onMousePressed(sf::Mouse::Button button)
{
    if (isActive() && button == sf::Mouse::Button::Left) {
        data->buttonState = BasicButtonState_Down;
        onButtonStateChanged();
        pressed();

        auto style = std::dynamic_pointer_cast<BasicButtonStyle>(getWidgetStyle());
        if (!style)
            return;

        setBackgroundColor(style->pressedColor);
    }
}

void BasicButton::onMouseReleased(sf::Mouse::Button button)
{
    if (isActive() && button == sf::Mouse::Button::Left) {
        data->buttonState = BasicButtonState_Normal;
        onButtonStateChanged();
        released();

        auto style = std::dynamic_pointer_cast<BasicButtonStyle>(getWidgetStyle());
        if (!style)
            return;

        setBackgroundColor(style->getHoverColor());
    }
}

void BasicButton::onButtonStateChanged()
{

}

void BasicButton::onStyleChanged()
{
    Widget::onStyleChanged();

    auto style = std::dynamic_pointer_cast<BasicButtonStyle>(getWidgetStyle());
    if (!style)
        return;

    auto color = style->normalColor;
    if (data->buttonState == BasicButtonState_Down)
        color = style->pressedColor;

    setBackgroundColor(color);
}

