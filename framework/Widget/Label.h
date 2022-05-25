#pragma once
#include <SFML/Graphics/Text.hpp>
#include <Widget/Widget.h>

class Label : public Widget
{
public:
    Label(const sf::Vector2f &size = sf::Vector2f(),
          const std::optional<CornerStyle> &cornerStyle = std::optional<CornerStyle>());
    ~Label();
public:
    void setText(const sf::String &text);
    sf::String getText()const;
protected:
    void onDrawObject(sf::RenderTarget &target, sf::RenderStates states) const override;
    void onPositionChanged() override;
    void onStyleChanged() override;
private:
    std::unique_ptr<class LabelData> data;
};
