#pragma once
#include <Widget/BasicButton.h>

class Button : public BasicButton
{
public:
    Button(const sf::Vector2f &size = sf::Vector2f(96, 36),
           const std::optional<CornerStyle> &cornerStyle = std::optional<CornerStyle>());
    ~Button();
public:
    void setText(const sf::String &text);
    sf::String getText()const;
private:
    void onDrawWidget(sf::RenderTarget &target, sf::RenderStates states)const override;
    void onPositionChanged() override;
    void onStyleChanged() override;
    void onButtonStateChanged() override;

    void recalcTextPosition(float offsetx, float offsety);
private:
    std::unique_ptr<class ButtonData> data;
};
