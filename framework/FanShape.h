#pragma once
#include <memory>
#include <SFML/Graphics/Shape.hpp>
#include <Util.h>

class FanShape : public sf::Shape
{
public:
    FanShape(float radius = 60.0f, const RangeFloat& rangle = RangeFloat(0.0f, 180.0f));
    ~FanShape() override;
public:
    void setRadius(float radius);
    float getRaduis()const;

    void setRangeAngle(float start, float end);
    void setRangeAngle(const Range<float>& rangle);
    RangeFloat& getRangleAngle()const;
public:
    std::size_t getPointCount() const override;
    sf::Vector2f getPoint(std::size_t index) const override;
private:
    std::unique_ptr<class FanShapeData> data;
};
