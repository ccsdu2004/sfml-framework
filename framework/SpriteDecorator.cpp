#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SpriteDecorator.h>
#include <FanShape.h>
#include <Sprite.h>
#include <Util.h>
#include <iostream>

class SpriteRingData
{
public:
    SpriteRingData()
    {
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineThickness(2.4f);
        circle.setOutlineColor(sf::Color::Blue);
    }

    float scaleFactor = 1.0f;
    sf::CircleShape circle;
};

SpriteRingDecorator::SpriteRingDecorator():
    data(new SpriteRingData())
{
}

SpriteRingDecorator::~SpriteRingDecorator()
{
}

void SpriteRingDecorator::setScaleFactor(float factor)
{
    if (factor > 0.0f)
        data->scaleFactor = factor;
}

float SpriteRingDecorator::getScaleFactor()const
{
    return data->scaleFactor;
}

void SpriteRingDecorator::setOutlineThickness(float thickness)
{
    data->circle.setOutlineThickness(thickness);
}

float SpriteRingDecorator::getOutlineThickness()const
{
    return data->circle.getOutlineThickness();
}

void SpriteRingDecorator::setOutlineColor(const sf::Color &color)
{
    data->circle.setOutlineColor(color);
}

sf::Color SpriteRingDecorator::getOutlineColor()const
{
    return data->circle.getOutlineColor();
}

void SpriteRingDecorator::setFillColor(const sf::Color &color)
{
    data->circle.setFillColor(color);
}

sf::Color SpriteRingDecorator::getFillColor()const
{
    return data->circle.getFillColor();
}

void SpriteRingDecorator::update(const Sprite *sprite)
{
    if (!sprite)
        return;

    auto box = sprite->getBoundingBox();
    auto center = sprite->getPosition();
    float radius = distance(sf::Vector2f(box.left, box.top), center) * data->scaleFactor;

    data->circle.setPosition(sf::Vector2f(-radius + box.width * 0.5f, -0.5f * box.height));
    data->circle.setRadius(radius);
}

void SpriteRingDecorator::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(data->circle, states);
}

class SpriteFanShapeDecoratorData
{
public:
    FanShape fan;
    float includeAngle = 60.0f;
};

SpriteFanShapeDecorator::SpriteFanShapeDecorator():
    data(new SpriteFanShapeDecoratorData())
{
}

SpriteFanShapeDecorator::~SpriteFanShapeDecorator()
{
}

void SpriteFanShapeDecorator::setRadius(float radius)
{
    if (radius < .0f)
        return;
    data->fan.setRadius(radius);
}

float SpriteFanShapeDecorator::getRadius() const
{
    return data->fan.getRaduis();
}

void SpriteFanShapeDecorator::setIncludedAngle(float angle)
{
    if (angle <= 0.0f)
        return;

    data->includeAngle = angle;
}

float SpriteFanShapeDecorator::getIncludedAngle()const
{
    return data->includeAngle;
}

void SpriteFanShapeDecorator::setFillColor(const sf::Color &color)
{
    data->fan.setFillColor(color);
}

sf::Color SpriteFanShapeDecorator::getFillColor()const
{
    return data->fan.getFillColor();
}

void SpriteFanShapeDecorator::update(const Sprite *sprite)
{
    if (!sprite)
        return;

    float angle = sprite->getRotate();
    float radius = data->fan.getRaduis();

    data->fan.setRangeAngle(angle - data->includeAngle * 0.5f,
                            angle + data->includeAngle * 0.5f);
}

void SpriteFanShapeDecorator::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(data->fan, states);
}

