#include <Widget/BoxLayout.h>

class HBoxLayoutInfo : public WidgetLayoutInfo
{
public:
    HBoxLayoutInfo(VMode inputMode):
        mode(inputMode)
    {
    }

    VMode mode = VMode_Center;
};

class HBoxLayoutData
{
public:
    HBoxLayoutData(HBoxLayout &inputLayout):
        layout(inputLayout)
    {
    }

    void adjustLayoutHeight()
    {
        auto count = layout.getWidgetCount();
        if(count == 0)
            return;

        float maxWidth = 0.0f;
        maxHeight = 0.0f;
        for (uint32_t i = 0; i < count; i++) {
            auto item = layout.getWidgetByIndex(i);
            auto widget = item.first;
            auto info = item.second;
            maxWidth += widget->getWidth();
            maxHeight = std::max(maxHeight, widget->getHeight());
        }

        maxWidth += layout.getMargin() * 2.0f + (count - 1) * layout.getSpacing();
        maxHeight = std::max(maxHeight, 20.0f);
        layout.setSize(maxWidth, maxHeight + 2 * layout.getMargin());
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

            currentWidgetCenter += widget->getWidth() * 0.5f;

            auto yoffset = widget->getHeight() * 0.5f + layout.getMargin();
            if (info && std::dynamic_pointer_cast<HBoxLayoutInfo>(info)) {
                auto hboxInfo = std::dynamic_pointer_cast<HBoxLayoutInfo>(info);
                if (hboxInfo->mode == VMode_Bottom)
                    yoffset += layout.getHeight() - widget->getHeight() - 2 * layout.getMargin();
                else if (hboxInfo->mode == VMode_Center)
                    yoffset = 0.5f * layout.getHeight();
            }

            widget->setCenter(sf::Vector2f(currentWidgetCenter, yoffset));

            currentWidgetCenter += layout.getSpacing();
            currentWidgetCenter += widget->getWidth() * 0.5f;
        }
    }
private:
    HBoxLayout &layout;
    float maxHeight = 0.0f;
    sf::Vector2f currentPosition;
};

HBoxLayout::HBoxLayout():
    data(new HBoxLayoutData(*this))
{
}

HBoxLayout::~HBoxLayout()
{
}

void HBoxLayout::addWidget(WidgetPointer widget, VMode mode)
{
    auto info = std::make_shared<HBoxLayoutInfo>(mode);
    WidgetLayout::addWidget(widget, info);
}

void HBoxLayout::adjust()
{
    data->adjustLayoutHeight();
    data->adjustWidgets();
}
