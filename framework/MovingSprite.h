#pragma once
#include <Sprite.h>

class MovingSprite : public Sprite
{
public:
    MovingSprite();
    ~MovingSprite();
public:
    void setAcclerate(const sf::Vector2f& accelerate);
    sf::Vector2f getAccelerate()const;

    void setVelocity(const sf::Vector2f& velocity);
    sf::Vector2f getVelocity()const;
private:
    void onUpdateObject(float deltaTime) override;
private:
    std::unique_ptr<class MovingSpriteData> data;
};
