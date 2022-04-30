#include <MovingSprite.h>

class MovingSpriteData
{
public:
    sf::Vector2f velocity;
    sf::Vector2f accelerate;
};

MovingSprite::MovingSprite():
    data(new MovingSpriteData)
{
}

MovingSprite::~MovingSprite()
{

}

void MovingSprite::setAcclerate(const sf::Vector2f &accelerate)
{
    data->accelerate = accelerate;
}

sf::Vector2f MovingSprite::getAccelerate() const
{
    return data->accelerate;
}

void MovingSprite::setVelocity(const sf::Vector2f& velocity)
{
    data->velocity = velocity;
}

sf::Vector2f MovingSprite::getVelocity()const
{
    return data->velocity;
}

void MovingSprite::onUpdateObject(float deltaTime)
{
    Sprite::onUpdateObject(deltaTime);
    sf::Vector2f delta(data->velocity * deltaTime);
    move(delta.x, delta.y);
    data->velocity += data->accelerate * deltaTime;
}

