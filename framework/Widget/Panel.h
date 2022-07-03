#pragma once
#include <Widget/BoxLayout.h>

class Panel : public VBoxLayout
{
public:
    Panel();
    virtual ~Panel();
public:
    void setTitle(const sf::String& title);
    sf::String getTitle()const;

    void setContextWidget(std::shared_ptr<Widget> widget);
private:
    void onStyleChanged() override;
    void onSizeChanged() override;
private:
    std::unique_ptr<class PanelData> data;
};
