#pragma once
#include <Sprite.h>

class SpriteDecorator
{
public:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states, const Sprite* sprite) = 0;
};

class SpriteRing : public SpriteDecorator
{
public:
    SpriteRing();
    ~SpriteRing();
public:
    void setScaleFactor(float factor);
    float getScaleFactor()const;

    void setOutlineThickness(float thickness);
    float getOutlineThickness()const;

    void setOutlineColor(const sf::Color& color);
    sf::Color getOutlineColor()const;

    void setFillColor(const sf::Color& color);
    sf::Color getFillColor()const;
public:
    void draw(sf::RenderTarget &target, sf::RenderStates states, const Sprite* sprite) override;
private:
    std::unique_ptr<class SpriteRingData> data;
};



