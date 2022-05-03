#pragma once
#include <optional>
#include <memory>
#include <SFML/Graphics/Color.hpp>
#include <Object.h>
#include <CornerStyle.h>

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
    static sf::Vector2f adjustPosition(const sf::FloatRect &box,
                                       const sf::Vector2f &target, HMode h, VMode v, float xmargin = 3.0f, float ymargin = 3.0f);

    Entity(const sf::Vector2f &size = sf::Vector2f(),
           const std::optional<CornerStyle> &cornerStyle = std::optional<CornerStyle>());
    virtual ~Entity();
public:
    void setPosition(float x, float y);
    void setPosition(const sf::Vector2f &position);
    sf::Vector2f getPosition()const;

    void move(float dx, float dy);
    void move(float distance);

    void setRotate(float angle);
    float getRotate()const;

    void setScale(float scale);
    float getScale()const;

    void setSize(float width, float height);
    sf::Vector2f getSize()const;

    sf::FloatRect getBoundingBox()const;

    sf::Transform getTransform()const;

    void setBackgroundColor(const sf::Color &color);
    sf::Color getBackgroundColor()const;

    void setOutlineColor(const sf::Color &color);
    void setOutlineThickness(float thickness = 1.0f);

    void setZValue(uint32_t z);
    uint32_t getZValue()const;
protected:
    virtual void onUpdateObject(float deltaTime) override;
    virtual void onDrawObject(sf::RenderTarget &target, sf::RenderStates states) const override;

    virtual void onMouseEnter();
    virtual void onMouseExit();
    virtual void onMousePressed(sf::Mouse::Button button);
    virtual void onMouseReleased(sf::Mouse::Button button);
    virtual void onMouseWheelScroll(float scroll);
    virtual void onMouseMoved(int x, int y);

    virtual void onPositionChanged();
    virtual void onSizeChanged();

    virtual void onScaleChanged();
private:
    std::unique_ptr<class EntityImpl> data;
};


