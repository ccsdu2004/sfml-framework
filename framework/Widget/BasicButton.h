#pragma once
#include <boost/signals2.hpp>
#include <Widget/Widget.h>

enum BasicButtonState {
    BasicButtonState_Normal = 0,
    BasicButtonState_Down,
    BasicButtonState_Hover
};

class BasicButton : public Widget
{
public:
    BasicButton(const sf::Vector2f &size = sf::Vector2f(96, 36),
                const std::optional<CornerStyle> &cornerStyle = std::optional<CornerStyle>());
    ~BasicButton();
public:
    boost::signals2::signal<void()> pressed;
    boost::signals2::signal<void()> released;

    BasicButtonState getButtonState()const;
protected:
    virtual void onMouseEnter() override;
    virtual void onMouseExit() override;
    virtual void onMousePressed(sf::Mouse::Button button) override;
    virtual void onMouseReleased(sf::Mouse::Button button) override;

    virtual void onButtonStateChanged();
    virtual void onStyleChanged() override;
private:
    std::unique_ptr<class BasicButtonData> data;
};

