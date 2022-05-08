#pragma once
#include <Sprite.h>
#include <SFML/Graphics/Drawable.hpp>

class SpriteDecorator : public sf::Drawable
{
public:
    virtual void update(const Sprite *sprite) = 0;
};

class SpriteRingDecorator : public SpriteDecorator
{
public:
    SpriteRingDecorator();
    ~SpriteRingDecorator();
public:
    void setScaleFactor(float factor);
    float getScaleFactor()const;

    void setOutlineThickness(float thickness);
    float getOutlineThickness()const;

    void setOutlineColor(const sf::Color &color);
    sf::Color getOutlineColor()const;

    void setFillColor(const sf::Color &color);
    sf::Color getFillColor()const;
public:
    void update(const Sprite *sprite) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states)const override;
private:
    std::unique_ptr<class SpriteRingData> data;
};

class SpriteFanShapeDecorator : public SpriteDecorator
{
public:
    SpriteFanShapeDecorator();
    ~SpriteFanShapeDecorator();
public:
    void setRadius(float radius);
    float getRadius()const;

    void setIncludedAngle(float angle);
    float getIncludedAngle()const;

    void setFillColor(const sf::Color &color);
    sf::Color getFillColor()const;
public:
    void update(const Sprite *sprite) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
private:
    std::unique_ptr<class SpriteFanShapeDecoratorData> data;
};

