#pragma once
#include <memory>
#include <SFML/Graphics/Color.hpp>
#include <Object.h>

enum HMode : int32_t {
    HMode_Left = 0,
    HMode_Center,
    HMode_Right
};

enum VMode : int32_t {
    VMode_Top = 0,
    VMode_Center,
    VMode_Bottom
};

enum Orientation {
    Orientation_H = 0,
    Orientation_V
};

class Entity : public Object
{
    friend class EntityMouseListener;
public:
    static sf::Vector2f adjustPosition(const sf::FloatRect& box,
                                       const sf::Vector2f &target, HMode h, VMode v, float xmargin = 3.0f, float ymargin = 3.0f);

    Entity(const sf::Vector2f &size = sf::Vector2f());
    virtual ~Entity();
public:
    void setParent(std::shared_ptr<Entity> parent);
    std::shared_ptr<Entity> getParent()const;
public:
    void setPosition(float x, float y);
    void setPosition(const sf::Vector2f& position);
    sf::Vector2f getPosition()const;

    void move(float dx, float dy);

    void setRotate(float angle);
    float getRotate()const;

    void setSize(float width, float height);
    sf::Vector2f getSize()const;

    sf::Vector2f getCenter()const;
    sf::FloatRect getBoundingBox()const;

    sf::Transform getTransform()const;

    void setBackgroundColor(const sf::Color &color);
    sf::Color getBackgroundColor()const;

    void setOutlineColor(const sf::Color &color);
    void setOutlineThickness(float thickness = 1.0f);
public:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void update(const sf::Time &time) override;
protected:
    virtual void onUpdate(const sf::Time &time);
    virtual void onDraw(sf::RenderTarget &target, sf::RenderStates states)const;

    virtual void onMouseEnter();
    virtual void onMouseExit();
    virtual void onMousePressed(sf::Mouse::Button button);
    virtual void onMouseReleased(sf::Mouse::Button button);
    virtual void onMouseWheelScroll(float scroll);
    virtual void onMouseMoved(int x, int y);

    virtual void onPositionChanged();
    virtual void onSizeChanged();
private:
    std::unique_ptr<class EntityImpl> data;
};


