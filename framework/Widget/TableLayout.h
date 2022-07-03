#pragma once
#include <Widget/WidgetLayout.h>

class TableLayout : public WidgetLayout
{
public:
    TableLayout() = delete;
    TableLayout(uint32_t row, uint32_t col);
    ~TableLayout();
public:
    void addWidget(std::shared_ptr<Widget> widget, uint32_t row, uint32_t col, HMode hMode = HMode_Center, VMode vMode = VMode_Center);
protected:
    void adjust()override;
private:
    std::unique_ptr<class TableLayoutData> data;
};
