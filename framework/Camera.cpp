#include <Camera.h>
#include <Entity.h>
#include <Application.h>

class CameraData
{
public:
    CameraData(Camera& inputCamera):
        camera(inputCamera)
    {
    }

    void onTargetPositionChanged(const sf::Vector2f& newPosition)
    {
        camera.setCenter(newPosition);
    }

    Camera& camera;
    sf::View view;
    bool dirty = false;
    sf::FloatRect globalArea;
    std::weak_ptr<Entity> target;
};

Camera::Camera():
    data(new CameraData(*this))
{
}

Camera::~Camera()
{
}

sf::View Camera::getView() const
{
    return data->view;
}

void Camera::setWindowArea(const sf::FloatRect &area)
{
    data->view.reset(area);
    if(data->globalArea.width < 1.0f)
        data->globalArea = area;
    data->dirty = true;
}

void Camera::setGlobalArea(const sf::FloatRect &area)
{
    data->globalArea = area;
}

void Camera::setCenter(const sf::Vector2f &center)
{
    auto size = data->view.getSize();
    auto oldCenter = data->view.getCenter();

    bool updateX = false;
    bool updateY = false;

    if(center.x - size.x * 0.5f >= data->globalArea.left &&
       center.x + size.x * 0.5f <= data->globalArea.left + data->globalArea.width) {
        updateX = true;
    }

    if(center.y - size.y * 0.5f >= data->globalArea.top &&
       center.y + size.y * 0.5f <= data->globalArea.top + data->globalArea.height)
        updateY = true;

    if(updateX == false && updateY == false)
        return;

    auto newCenter = center;
    if(!updateX)
        newCenter.x = oldCenter.x;
    if(!updateY)
        newCenter.y = oldCenter.y;

    data->view.setCenter(newCenter);
    data->dirty = true;
}

void Camera::setTarget(EntityPointer target)
{
    if(target) {
        data->target = target;
        target->positionChanged.connect(std::bind(&CameraData::onTargetPositionChanged, data.get(), std::placeholders::_1));
    }
}

void Camera::setViewport(const sf::FloatRect &area)
{
    data->view.setViewport(area);
    data->dirty = true;
}

void Camera::setRotate(float angle)
{
    data->view.setRotation(angle);
    data->dirty = true;
}

void Camera::update(float deltaTime)
{
    (void)deltaTime;
    if(data->dirty) {
        data->dirty = false;

        Application::getInstance()->getWindow()->setView(data->view);

        auto center = getView().getCenter();
        auto size = getView().getSize();
        auto leftTop = center - size * 0.5f;
        onChanged(leftTop);
    }
}
