#pragma once
#include <SFML/Graphics/Rect.hpp>
#include <Def.h>
#include <Component.h>

class MiniMap : public Component
{
public:
    MiniMap();
    ~MiniMap();
public:
    void setArea(const sf::FloatRect& area);
    void setViewport(const sf::FloatRect& viewport);
public:
    void draw(std::shared_ptr<Object> object);
    void update(float deltaTime) override;
private:
    std::unique_ptr<class MiniMapData> data;
};
