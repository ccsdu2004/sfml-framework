#include <Util.h>
#include <Widget/AbstractSlider.h>

class AbstractSliderData
{
public:
    int32_t value = 0;
    int32_t valueStep = 1;
    Range<int32_t> valueRange = Range<int32_t>(0, 100);
};

AbstractSlider::AbstractSlider():
    data(new AbstractSliderData())
{
}

AbstractSlider::~AbstractSlider()
{

}

void AbstractSlider::setRange(int32_t low, int32_t high)
{
    if(low > high)
        return;

    data->valueRange = Range<int32_t>(low, high);
    setValue(data->value);
}

int32_t AbstractSlider::getLow()const
{
    return data->valueRange.low;
}

int32_t AbstractSlider::getHigh()const
{
    return data->valueRange.high;
}

void AbstractSlider::setValue(int32_t value)
{
    if(value < data->valueRange.low) {
        data->value = data->valueRange.low;
        onValueChanged();
    } else if(value > data->valueRange.high) {
        data->value = data->valueRange.high;
        onValueChanged();
    } else if(data->value != value) {
        data->value = value;
        onValueChanged();
    }
}

int32_t AbstractSlider::getValue()const
{
    return data->value;
}

void AbstractSlider::setStep(int32_t value)
{
    data->valueStep = value;
}

int32_t AbstractSlider::getStep()const
{
    return data->valueStep;
}

float AbstractSlider::getValuePercent() const
{
    return (data->value - data->valueRange.low) / float(data->valueRange.high - data->valueRange.low);
}

void AbstractSlider::onValueChanged()
{
}
