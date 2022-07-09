#include <Camera.h>
#include <Application.h>

class CameraData
{
public:
    sf::View view;
    bool dirty = false;
};

Camera::Camera():
    data(new CameraData)
{
}

Camera::~Camera()
{
}

sf::View Camera::getView() const
{
    return data->view;
}

void Camera::setArea(const sf::FloatRect &area)
{
    data->view.reset(area);
    data->dirty = true;
}

void Camera::setCenter(const sf::Vector2f &center)
{
    data->view.setCenter(center);
    data->dirty = true;
}

void Camera::setViewport(const sf::FloatRect &area)
{
    data->view.setViewport(area);
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
