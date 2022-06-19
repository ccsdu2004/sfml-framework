#pragma once
#include <Widget/Widget.h>
#include <Widget/WidgetStyle.h>

class WidgetLayoutInfo
{
public:
    virtual ~WidgetLayoutInfo()
    {
    }
};

class WidgetLayout : public Widget
{
public:
    WidgetLayout();
    virtual ~WidgetLayout();
public:
    void setMargin(float margin);
    float getMargin()const;

    void setSpacing(float spacing);
    float getSpacing()const;
public:
    void addWidget(WidgetPointer widget, std::shared_ptr<WidgetLayoutInfo> info);

    uint32_t getWidgetCount()const;
    std::pair<WidgetPointer, std::shared_ptr<WidgetLayoutInfo>> getWidgetByIndex(uint32_t index);
public:
    virtual WidgetPointer getWidgetBelow(const sf::Vector2i &point)override;
    virtual bool doesHierarchyContain(WidgetPointer other)const override;
protected:
    void onActiveChanged()override;
    void onVisibleChanged()override;
    void onGainFocus()override;
    void onLostFocus()override;
    void onMouseExit() override;
    void onMovableChanged()override;

    void onPositionChanged() override;
    void onSizeChanged() override;

    void onUpdateObject(float deltaTime) override;
    virtual void adjust() = 0;
private:
    std::unique_ptr<class WidgetLayoutData> data;
};

