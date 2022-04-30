#pragma once
#include <Component.h>
#include <Widget/Widget.h>
#include <Widget/WidgetStyle.h>

class Desktop : public sf::Drawable, public MessageReceiver
{
public:
    Desktop();
    ~Desktop();
public:
    void addWidget(WidgetPointer widget, int x, int y);
    void removeWidget(WidgetPointer widget);
    void clearAllWidgets();
public:
    WidgetPointer getFocusWidget()const;
    void setFocusWidget(WidgetPointer widget);

    WidgetPointer getPopupWidget()const;
    void setPopupWidget(WidgetPointer widget);

    WidgetPointer widgetBelowCursor()const;
    bool isTopWidget(WidgetPointer widget)const;

    ComponentPoolPointer getStylePointer()const;
public:
    bool process(std::shared_ptr<Message> message) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
private:
    std::unique_ptr<class DesktopData> data;
};
