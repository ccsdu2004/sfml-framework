#include <Camera.h>

class CameraData
{
public:
    CameraData()
    {
        view.setCenter(0, 0);
        view.setSize(640.0f, 480.0f);
    }

    sf::View view;
};

Camera::Camera():
    data(new CameraData)
{
}

Camera::~Camera()
{
}

sf::View& Camera::getView()const
{
    return data->view;
}

bool Camera::needRemoved() const
{

}

void Camera::onDrawObject(sf::RenderTarget &target, sf::RenderStates states) const
{

}

void Camera::onUpdateObject(float deltaTime)
{

}
