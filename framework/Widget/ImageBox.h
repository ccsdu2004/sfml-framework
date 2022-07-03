#pragma once
#include <boost/signals2.hpp>
#include <Widget/Widget.h>
#include <Widget/WidgetGroup.h>
#include <Widget/WidgetSelector.h>

class ImageBox : public Widget, public WidgetSelector
{
public:
    ImageBox(const sf::Vector2f& size = sf::Vector2f());
    ~ImageBox();
public:
    void setTexture(const sf::Texture &texture);
    const sf::Texture* getTexture()const;

    boost::signals2::signal<void(std::shared_ptr<ImageBox>)> clicked;
    void setWidgetGroup(std::shared_ptr<WidgetGroup> group);

    void select() override;
    void unselect() override;
protected:
    void onMousePressed(sf::Mouse::Button button) override;
    void onMouseReleased(sf::Mouse::Button button) override;

    void onStyleChanged() override;
    void onDrawWidget(sf::RenderTarget &target, sf::RenderStates states) const override;
    void onPositionChanged() override;
    void onSizeChanged() override;
    void onScaleChanged() override;

    void onUpdateObject(float deltaTime) override;
private:
    std::unique_ptr<class ImageBoxData> data;
};
