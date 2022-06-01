#pragma once
#include <Component.h>
#include <Widget/Widget.h>
#include <Widget/WidgetStyle.h>

class Desktop : public sf::Drawable, public MessageReceiver, public Component
{
public:
    Desktop();
    ~Desktop();
public:
    void addWidget(WidgetPointer widget, float x, float y);
    void addWidget(WidgetPointer widget, HMode hmode, VMode vmode, float xoffset = 3.0f,
                   float yoffset = 3.0f);
    void removeWidget(WidgetPointer widget);
    void clearAllWidgets();
public:
    WidgetPointer getFocusWidget()const;
    void setFocusWidget(WidgetPointer widget);

    WidgetPointer getPopupWidget()const;
    void setPopupWidget(WidgetPointer widget);

    WidgetPointer widgetBelowCursor()const;
    bool isTopWidget(WidgetPointer widget)const;

    std::shared_ptr<WidgetStyle> getWidgetStyle(const std::string &name);
public:
    bool process(std::shared_ptr<Message> message) override;
    void update(float deltaTime) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
private:
    std::unique_ptr<class DesktopData> data;
};
