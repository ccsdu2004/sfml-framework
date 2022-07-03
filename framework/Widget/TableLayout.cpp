#include <numeric>
#include <Widget/TableLayout.h>

class TableLayoutInfo : public WidgetLayoutInfo
{
public:
    std::shared_ptr<Widget> widget;
    uint32_t row = 0;
    uint32_t col = 0;

    HMode hMode = HMode_Center;
    VMode vMode = VMode_Center;
};

class TableLayoutData
{
public:
    TableLayoutData(TableLayout &inputLayout):
        layout(inputLayout)
    {
    }

    void adjustWidgets()
    {
        auto count = layout.getWidgetCount();
        if (count == 0)
            return;

        auto width = std::accumulate(colWidths.begin(), colWidths.end(), 0);
        auto height = std::accumulate(rowHeights.begin(), rowHeights.end(), 0);

        width += 2 * layout.getMargin();
        height += 2 * layout.getMargin();

        if(colWidths.size() >= 2)
            width += (colWidths.size() - 1) * layout.getSpacing();

        if(rowHeights.size() >= 2)
            height += (rowHeights.size() - 1) * layout.getSpacing();

        layout.setFixedWidth(width);
        layout.setFixedHeight(height);

        auto getXByColIndex = [&](uint32_t col)->float {
            float x = layout.getMargin();
            for(uint32_t i = 0; i < col; i++)
            {
                x += colWidths[i];
                x += layout.getSpacing();
            }

            return x;
        };

        auto getYByRowIndex = [&](uint32_t row)->float {
            float y = layout.getMargin();
            for(uint32_t i = 0; i < row; i++)
            {
                y += rowHeights[i];
                y += layout.getSpacing();
            }

            return y;
        };

        for (uint32_t i = 0; i < layout.getWidgetCount(); i++) {
            auto item = layout.getWidgetByIndex(i);
            auto widget = item.first;
            auto info = item.second;
            auto tableInfo = std::dynamic_pointer_cast<TableLayoutInfo>(info);
            if(tableInfo) {
                auto x = getXByColIndex(tableInfo->col);
                auto y = getYByRowIndex(tableInfo->row);

                sf::FloatRect area(x, y, colWidths[tableInfo->col], rowHeights[tableInfo->row]);

                auto pos = Entity::adjustPosition(area, sf::Vector2f(tableInfo->widget->getSize()), tableInfo->hMode, tableInfo->vMode);
                tableInfo->widget->setPosition(pos);
            }
        }
    }
public:
    std::vector<float> rowHeights;
    std::vector<float> colWidths;
private:
    TableLayout &layout;
};

TableLayout::TableLayout(uint32_t row, uint32_t col):
    data(new TableLayoutData(*this))
{
    data->rowHeights = std::vector<float>(row, 0.0f);
    data->colWidths = std::vector<float>(col, 0.0f);
}

TableLayout::~TableLayout()
{

}

void TableLayout::addWidget(std::shared_ptr<Widget> widget, uint32_t row, uint32_t col, HMode hMode, VMode vMode)
{
    if(row >= data->rowHeights.size() || col >= data->colWidths.size())
        return;

    std::shared_ptr<TableLayoutInfo> info = std::make_shared<TableLayoutInfo>();
    info->widget = widget;
    info->row = row;
    info->col = col;
    info->hMode = hMode;
    info->vMode = vMode;
    WidgetLayout::addWidget(widget, std::dynamic_pointer_cast<WidgetLayoutInfo>(info));

    data->colWidths[col] = std::max(data->colWidths[col], widget->getWidth());
    data->rowHeights[row] = std::max(data->rowHeights[row], widget->getHeight());
}

void TableLayout::adjust()
{
    data->adjustWidgets();
}
