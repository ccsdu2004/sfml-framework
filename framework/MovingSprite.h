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

    void setRotateVelocity(float velocity);
    float getRotateVelocity()const;
private:
    void onUpdateObject(float deltaTime) override;
    virtual void onUpdateMovingSprite(float deltaTime);
private:
    std::unique_ptr<class MovingSpriteData> data;
};
