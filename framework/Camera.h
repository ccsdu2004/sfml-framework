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
    void setArea(const sf::FloatRect& area);
    void setCenter(const sf::Vector2f& center);
    void setViewport(const sf::FloatRect& area);
    void update(float deltaTime) override;

    boost::signals2::signal<void(const sf::Vector2f&)> onChanged;
private:
    std::unique_ptr<class CameraData> data;
};
