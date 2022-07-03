#pragma once
#include <Widget/AbstractSlider.h>

class ProgressBar : public AbstractSlider
{
public:
    ProgressBar();
    ~ProgressBar();
public:
    void setTextVisible(bool visible);
    bool isTextVisible()const;

    void setOrientation(Orientation o);
    Orientation getOrientation()const;
protected:
    void onStyleChanged() override;
    void onDrawWidget(sf::RenderTarget &target, sf::RenderStates states) const override;
    void onPositionChanged() override;
    void onValueChanged() override;
private:
    std::unique_ptr<class ProgressBarData> data;
};
