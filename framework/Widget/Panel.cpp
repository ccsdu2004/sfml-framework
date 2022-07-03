#include <Widget/Panel.h>
#include <Widget/MouseArea.h>
#include <Widget/BoxLayout.h>
#include <Widget/Desktop.h>
#include <Application.h>
#include <iostream>

class PanelData
{
public:
    std::shared_ptr<MouseArea> mouseArera;
    std::shared_ptr<Widget> contentWidget;
};

Panel::Panel():
    data(new PanelData())
{
    data->mouseArera = std::make_shared<MouseArea>();
    setMargin(1.0f);

    auto style = std::dynamic_pointer_cast<PanelStyle>(
                     Application::getInstance()->getComponent<Desktop>()->getWidgetStyle("PanelStyle"));
    assert(style);
    setWidgetStyle(style);
    data->mouseArera->setWidgetStyle(style->titleStyle);
}

Panel::~Panel()
{

}

void Panel::setTitle(const sf::String& title)
{
    data->mouseArera->setText(title);
}

sf::String Panel::getTitle()const
{
    return data->mouseArera->getText();
}

void Panel::setContextWidget(std::shared_ptr<Widget> widget)
{
    if(!data->contentWidget && widget) {
        data->contentWidget = widget;

        data->mouseArera->setTargetWidget(std::dynamic_pointer_cast<Widget>(shared_from_this()));
        data->mouseArera->setSize(180, 20);
        addWidget(data->mouseArera);
        addWidget(data->contentWidget);
    }
}

void Panel::onStyleChanged()
{
    Widget::onStyleChanged();
    auto style = std::dynamic_pointer_cast<PanelStyle>(getWidgetStyle());
    if(style) {
        data->mouseArera->setWidgetStyle(style->titleStyle);
    }
}

void Panel::onSizeChanged()
{
    VBoxLayout::onSizeChanged();
    data->mouseArera->setWidth(getWidth() * 0.95f);
}

