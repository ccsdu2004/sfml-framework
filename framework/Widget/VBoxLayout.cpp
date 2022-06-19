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
        auto count = layout.getWidgetCount();
        if(count == 0)
            return;

        maxWidth = 0.0f;
        float maxHeight = 0.0f;

        for (uint32_t i = 0; i < count; i++) {
            auto item = layout.getWidgetByIndex(i);
            auto widget = item.first;
            auto info = item.second;
            maxWidth = std::max(maxWidth, widget->getWidth());
            maxHeight += widget->getHeight();
        }

        maxWidth = std::max(maxWidth, 20.0f);
        layout.setSize(maxWidth + 2 * layout.getMargin(), maxHeight + 2 * layout.getMargin() + layout.getSpacing() * (count - 1));
    }

    void adjustWidgets()
    {
        auto count = layout.getWidgetCount();
        if (count == 0)
            return;

        float currentWidgetCenter = layout.getMargin();

        for (uint32_t i = 0; i < count; i++) {
            auto item = layout.getWidgetByIndex(i);
            auto widget = item.first;
            auto info = item.second;

            currentWidgetCenter += widget->getHeight() * 0.5f;

            auto xoffset = widget->getWidth() * 0.5f + layout.getMargin();
            if (info && std::dynamic_pointer_cast<VBoxLayoutInfo>(info)) {
                auto vboxInfo = std::dynamic_pointer_cast<VBoxLayoutInfo>(info);
                if (vboxInfo->mode == HMode_Right)
                    xoffset += layout.getWidth() - widget->getWidth() - 2 * layout.getMargin();
                else if (vboxInfo->mode == HMode_Center)
                    xoffset = 0.5f * layout.getWidth();
            }

            widget->setCenter(sf::Vector2f(xoffset, currentWidgetCenter));
            currentWidgetCenter += layout.getSpacing() + widget->getHeight() * 0.5f;
        }

        /*
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

            widget->setCenter(sf::Vector2f(xoffset - widget->getWidth() * 0.5f, prevWidgetTop));
            prevWidgetHeight = widget->getHeight();
            prevWidgetTop = widget->getPosition().y + widget->getHeight() + layout.getSpacing();
        }

        layout.setHeight(prevWidgetTop - prevWidgetHeight * 0.5f);
        */
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
