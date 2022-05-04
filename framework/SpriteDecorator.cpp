#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SpriteDecorator.h>
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

SpriteRing::SpriteRing():
    data(new SpriteRingData())
{
}

SpriteRing::~SpriteRing()
{
}

void SpriteRing::setScaleFactor(float factor)
{
    if(factor > 0.0f)
        data->scaleFactor = factor;
}

float SpriteRing::getScaleFactor()const
{
    return data->scaleFactor;
}

void SpriteRing::setOutlineThickness(float thickness)
{
    data->circle.setOutlineThickness(thickness);
}

float SpriteRing::getOutlineThickness()const
{
    return data->circle.getOutlineThickness();
}

void SpriteRing::setOutlineColor(const sf::Color &color)
{
    data->circle.setOutlineColor(color);
}

sf::Color SpriteRing::getOutlineColor()const
{
    return data->circle.getOutlineColor();
}

void SpriteRing::setFillColor(const sf::Color& color)
{
    data->circle.setFillColor(color);
}

sf::Color SpriteRing::getFillColor()const
{
    return data->circle.getFillColor();
}

void SpriteRing::draw(sf::RenderTarget &target, sf::RenderStates states, const Sprite* sprite)
{
    (void) states;

    if(!sprite)
        return;

    auto box = sprite->getBoundingBox();
    auto center = sprite->getPosition();
    float radius = distance(sf::Vector2f(box.left, box.top), center);
    radius *= data->scaleFactor;

    data->circle.setRadius(radius);
    data->circle.setPosition(sprite->getPosition() - sf::Vector2f(radius, radius));

    target.draw(data->circle);
}

