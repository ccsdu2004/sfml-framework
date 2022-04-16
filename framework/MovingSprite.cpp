#include <MovingSprite.h>

class MovingSpriteData
{
public:
    sf::Vector2f speed;
};

MovingSprite::MovingSprite():
    data(new MovingSpriteData)
{
}

MovingSprite::~MovingSprite()
{

}

void MovingSprite::setSpeed(const sf::Vector2f& speed)
{
    data->speed = speed;
}

sf::Vector2f MovingSprite::getSpeed()const
{
    return data->speed;
}

void MovingSprite::onUpdate(const sf::Time &time)
{
    float diff = time.asSeconds();
    sf::Vector2f delta(data->speed.x * diff, data->speed.y * diff);
    move(delta.x, delta.y);
}

