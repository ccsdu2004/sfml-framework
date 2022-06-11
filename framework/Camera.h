#pragma once
#include <SFML/Graphics/View.hpp>
#include <Object.h>

class Camera : public Object
{
public:
    Camera();
    ~Camera();
public:
    sf::View& getView()const;
    bool needRemoved()const override;
protected:
    void onDrawObject(sf::RenderTarget &target, sf::RenderStates states) const override;
    void onUpdateObject(float deltaTime) override;
private:
    std::unique_ptr<class CameraData> data;
};
