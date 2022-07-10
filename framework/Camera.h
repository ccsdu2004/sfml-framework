#pragma once
#include <boost/signals2.hpp>
#include <SFML/Graphics/View.hpp>
#include <Component.h>
#include <Object.h>

class Camera : public Component
{
public:
    Camera();
    ~Camera();
public:
    sf::View getView()const;
    void setWindowArea(const sf::FloatRect& area);
    void setGlobalArea(const sf::FloatRect& area);
    void setCenter(const sf::Vector2f& center);
    void setTarget(EntityPointer target);
    void setViewport(const sf::FloatRect& area);
    void setRotate(float angle);
    void update(float deltaTime) override;

    boost::signals2::signal<void(const sf::Vector2f&)> onChanged;
private:
    std::unique_ptr<class CameraData> data;
};
