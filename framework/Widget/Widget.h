#pragma once
#include <Entity.h>
#include <Util.h>

class Widget;
using WidgetPointer = std::shared_ptr<Widget>;

class Widget : public Entity
{
public:
    Widget(const sf::Vector2f &size = sf::Vector2f(),
           const std::optional<CornerStyle> &cornerStyle = std::optional<CornerStyle>());
    virtual ~Widget();
public:
    void setPadding(float padding);
    bool getPadding()const;

    void setWidth(float width);
    float getWidth()const;

    void setHeight(float height);
    float getHeight()const;
public:
    void setParent(std::weak_ptr<Widget> parent);
    std::weak_ptr<Widget> getParent()const;

    void setWidthRange(const RangeFloat &range);
    RangeFloat getWidthRange()const;

    void setHeightRange(const RangeFloat &range);
    RangeFloat getHeightRange()const;

    void setActive(bool active = true);
    bool isActive()const;

    void setVisible(bool visible = true);
    void show();
    void hide();
    bool isVisible()const;

    void setFocused(bool focus = true);
    bool isFocused()const;
public:
    bool isUnderMouse();
    bool containsPoint(const sf::Vector2i &point);
    virtual WidgetPointer getWidgetBelow(const sf::Vector2i &point);
    virtual bool doesHierarchyContain(WidgetPointer other)const;
protected:
    virtual void onActiveChanged();
    virtual void onVisibleChanged();
    virtual void onGainFocus();
    virtual void onLostFocus();

    virtual void onPositionChanged() override;
    virtual void onSizeChanged() override;

    virtual void onDrawObject(sf::RenderTarget &target, sf::RenderStates states) const override;
private:
    std::unique_ptr<class WidgetData> data;
};
