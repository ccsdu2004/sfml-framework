#pragma once
#include <Object.h>

class CameraTrackingObject : public Object
{
public:
    CameraTrackingObject();
    ~CameraTrackingObject();
public:
    void setTarget(std::shared_ptr<Entity> target);
    std::shared_ptr<Entity> getTarget()const;

    void setTargetPosition(const sf::Vector2f &position);
    sf::Vector2f getTargetPosition()const;
public:
    void onUpdateObject(float deltaTime) override;
private:
    std::unique_ptr<class CameraTrackingObjectData> data;
};
