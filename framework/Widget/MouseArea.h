#pragma once
#include <boost/signals2.hpp>
#include "Widget.h"

class MouseArea : public Widget
{
public:
    MouseArea(const sf::Vector2f &size = sf::Vector2f(),
              const std::optional<CornerStyle> &cornerStyle = std::optional<CornerStyle>());
    ~MouseArea();
public:
    void setTargetWidget(std::shared_ptr<Widget> target);
    std::shared_ptr<Widget> getTargetWidget()const;

    void setText(const sf::String& text);
    sf::String getText()const;

    boost::signals2::signal<void(void)> moved;
private:
    void onMouseEnter() override;
    void onMouseExit() override;
    void onMousePressed(sf::Mouse::Button button) override;
    void onMouseReleased(sf::Mouse::Button button) override;
    void onMouseMoved(int x, int y) override;

    void onStyleChanged() override;
private:
    std::unique_ptr<class MouseAreaData> data;
};
