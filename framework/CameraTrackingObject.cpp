#include <CameraTrackingObject.h>
#include <Camera.h>
#include <Entity.h>
#include <Application.h>

class CameraTrackingObjectData
{
public:
    CameraTrackingObjectData(CameraTrackingObject& object):
        cameraTrackingObject(object)
    {
    }

    void onCameraChanged(const sf::Vector2f& leftTop)
    {
        auto position = targetPosition + leftTop;
        target->setPosition(position);
    }

    std::shared_ptr<Entity> target;
    sf::Vector2f targetPosition;
    std::weak_ptr<Camera> camera;
    CameraTrackingObject& cameraTrackingObject;
};

CameraTrackingObject::CameraTrackingObject():
    data(new CameraTrackingObjectData(*this))
{
    auto camera = Application::getInstance()->getComponent<Camera>();
    if(camera) {
        data->camera = camera;
        auto fn = [this](const sf::Vector2f & leftTop) {
            data->onCameraChanged(leftTop);
        };
        camera->onChanged.connect(fn);
    }
}

CameraTrackingObject::~CameraTrackingObject()
{
}

void CameraTrackingObject::setTarget(std::shared_ptr<Entity> target)
{
    if(!target)
        return;

    if (data->target)
        removeChild(std::dynamic_pointer_cast<Object>(data->target));
    data->target = target;
    addChild(std::dynamic_pointer_cast<Object>(target));
}

std::shared_ptr<Entity> CameraTrackingObject::getTarget() const
{
    return data->target;
}

void CameraTrackingObject::setTargetPosition(const sf::Vector2f &position)
{
    data->targetPosition = position;
}

sf::Vector2f CameraTrackingObject::getTargetPosition() const
{
    return data->targetPosition;
}

void CameraTrackingObject::onUpdateObject(float deltaTime)
{
    (void)deltaTime;

    if(data->camera.expired()) {
        auto camera = Application::getInstance()->getComponent<Camera>();
        if(camera) {
            data->camera = camera;
            auto fn = [this](const sf::Vector2f & leftTop) {
                data->onCameraChanged(leftTop);
            };
            camera->onChanged.connect(fn);
        }
    }
}
