#pragma once
#include <Sprite.h>

class MovingSprite : public Sprite
{
public:
    MovingSprite();
    ~MovingSprite();
public:
    void setSpeed(const sf::Vector2f& speed);
    sf::Vector2f getSpeed()const;
private:
    void onUpdate(const sf::Time &time) override;
private:
    std::unique_ptr<class MovingSpriteData> data;
};
