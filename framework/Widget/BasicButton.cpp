#include <Application.h>
#include <Widget/Desktop.h>
#include <Widget/BasicButton.h>

class BasicButtonData
{
public:
    BasicButtonState buttonState = BasicButtonState::BasicButtonState_Normal;
    std::shared_ptr<BasicButtonStyle> style;
};

BasicButton::BasicButton(const sf::Vector2f &size, const std::optional<CornerStyle> &cornerStyle):
    Widget(size, cornerStyle),
    data(new BasicButtonData)
{
    auto stylePool = Application::getInstance()->getComponent<Desktop>()->getStylePointer();
    assert(stylePool);
    data->style = stylePool->getComponent<BasicButtonStyle>();
    assert(data->style);

    setWidgetStyle(data->style);
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
    /*if(isActive()) {
        data->buttonState = BasicButtonState_Hover;

        auto style = std::dynamic_pointer_cast<BasicButtonStyle>(getWidgetStyle());
        if(!style)
            return;

        setBackgroundColor(style->getHoverColor());
    }

    Widget::onMouseEnter();*/
}

void BasicButton::onMouseExit()
{
    /*if(isActive()) {
        data->buttonState = BasicButtonState_Normal;

        auto style = std::dynamic_pointer_cast<BasicButtonStyle>(getWidgetStyle());
        if(!style)
            return;

        setBackgroundColor(style->getActiveColor());
    }

    Widget::onMouseExit();*/
}

void BasicButton::onMousePressed(sf::Mouse::Button button)
{
    /*if(isActive() && button == sf::Mouse::Button::Left) {
        data->buttonState = BasicButtonState_Down;
        pressed();

        auto style = std::dynamic_pointer_cast<BasicButtonStyle>(getWidgetStyle());
        if(!style)
            return;

        setBackgroundColor(style->pressedColor);
    }*/
}

void BasicButton::onMouseReleased(sf::Mouse::Button button)
{
    /*if(isActive() && button == sf::Mouse::Button::Left) {
        data->buttonState = BasicButtonState_Normal;
        released();

        auto style = std::dynamic_pointer_cast<BasicButtonStyle>(getWidgetStyle());
        if(!style)
            return;

        setBackgroundColor(style->getHoverColor());
    }*/
}

void BasicButton::onStyleChanged()
{
    Widget::onStyleChanged();

    auto style = std::dynamic_pointer_cast<BasicButtonStyle>(getWidgetStyle());
    if(!style)
        return;

    auto color = style->getActiveColor();
    if(data->buttonState == BasicButtonState_Down)
        color = style->pressedColor;

    setBackgroundColor(color);
}

