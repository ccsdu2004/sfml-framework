#pragma once
#include <Widget/Widget.h>

class AbstractSlider : public Widget
{
public:
    AbstractSlider();
    virtual ~AbstractSlider();
public:
    void setRange(int32_t low, int32_t hight);
    int32_t getLow()const;
    int32_t getHigh()const;

    void setValue(int32_t value);
    int32_t getValue()const;
    void setStep(int32_t value = 1);
    int32_t getStep()const;

    float getValuePercent()const;
protected:
    virtual  void onValueChanged();
private:
    std::unique_ptr<class AbstractSliderData> data;
};
