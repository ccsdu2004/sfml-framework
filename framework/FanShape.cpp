#include <cmath>
#include <FanShape.h>

class FanShapeData
{
public:
    float radius;
    RangeFloat rangeAngle;
};

FanShape::FanShape(float radius, const RangeFloat &rangeAngle):
    data(new FanShapeData())
{
    data->radius = radius;
    setRangeAngle(rangeAngle);
}

FanShape::~FanShape()
{
}

void FanShape::setRadius(float radius)
{
    data->radius = radius;
    update();
}

float FanShape::getRaduis() const
{
    return data->radius;
}

void FanShape::setRangeAngle(float start, float end)
{
    setRangeAngle(RangeFloat(start, end));
}

void FanShape::setRangeAngle(const RangeFloat &rangle)
{
    data->rangeAngle = rangle;
    if(data->rangeAngle.isValid())
        std::swap(data->rangeAngle.high, data->rangeAngle.low);

    update();
}

RangeFloat &FanShape::getRangleAngle() const
{
    return data->rangeAngle;
}

std::size_t FanShape::getPointCount() const
{
    return 102;
}

sf::Vector2f FanShape::getPoint(std::size_t index) const
{
    if(index == 0)
        return sf::Vector2f(data->radius, data->radius);

    index -= 1;
    const float step = (data->rangeAngle.high - data->rangeAngle.low) * 0.01f;
    sf::Vector2f point(data->radius + data->radius * std::cos(data->rangeAngle.low / 180.0f * M_PI + step * index / 180.0f * M_PI - M_PI_2),
                       data->radius + data->radius * std::sin(data->rangeAngle.low / 180.0f * M_PI + step * index / 180.0f * M_PI - M_PI_2));

    return point;
}

