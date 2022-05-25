#include <Widget/BoxLayout.h>

class VBoxLayoutInfo : public WidgetLayoutInfo
{
public:
    VBoxLayoutInfo(HMode inputMode):
        mode(inputMode)
    {
    }

    HMode mode = HMode_Center;
};

class VBoxLayoutData
{
public:
    VBoxLayoutData(VBoxLayout &inputLayout):
        layout(inputLayout)
    {
    }

    void adjustLayoutWidth()
    {
        maxWidth = 0.0f;

        auto count = layout.getWidgetCount();
        for (uint32_t i = 0; i < count; i++) {
            auto item = layout.getWidgetByIndex(i);
            auto widget = item.first;
            auto info = item.second;
            maxWidth = std::max(maxWidth, widget->getWidth());
        }

        maxWidth = std::max(maxWidth, 20.0f);
        layout.setWidth(maxWidth + 2 * layout.getMargin());
    }

    void adjustWidgets()
    {
        auto count = layout.getWidgetCount();
        if (count == 0)
            return;

        float prevWidgetTop = layout.getWidgetByIndex(0).first->getHeight() * 0.5f + layout.getMargin();
        float prevWidgetHeight = 0.0f;

        for (uint32_t i = 0; i < count; i++) {
            auto item = layout.getWidgetByIndex(i);
            auto widget = item.first;
            auto info = item.second;

            auto xoffset = widget->getWidth() * 0.5f + layout.getMargin();
            if (info && std::dynamic_pointer_cast<VBoxLayoutInfo>(info)) {
                auto vboxInfo = std::dynamic_pointer_cast<VBoxLayoutInfo>(info);
                if (vboxInfo->mode == HMode_Right)
                    xoffset += layout.getWidth() - widget->getWidth() - 2 * layout.getMargin();
                else if (vboxInfo->mode == HMode_Center)
                    xoffset = 0.5f * layout.getWidth();
            }

            widget->setPosition(xoffset, prevWidgetTop);
            prevWidgetHeight = widget->getHeight();
            prevWidgetTop = widget->getPosition().y + widget->getHeight() + layout.getSpacing();
        }

        layout.setHeight(prevWidgetTop - prevWidgetHeight * 0.5f);
    }
private:
    VBoxLayout &layout;
    float maxWidth = 0.0f;
    sf::Vector2f currentPosition;
};

VBoxLayout::VBoxLayout():
    data(new VBoxLayoutData(*this))
{
}

VBoxLayout::~VBoxLayout()
{

}

void VBoxLayout::addWidget(WidgetPointer widget, HMode mode)
{
    auto info = std::make_shared<VBoxLayoutInfo>(mode);
    WidgetLayout::addWidget(widget, info);
}

void VBoxLayout::adjust()
{
    data->adjustLayoutWidth();
    data->adjustWidgets();
}
