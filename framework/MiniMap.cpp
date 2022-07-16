#include <SFML/Graphics/View.hpp>
#include <MiniMap.h>
#include <Camera.h>
#include <Application.h>
#include <iostream>

class MiniMapData
{
public:
    sf::View view;
};

MiniMap::MiniMap():
    data(new MiniMapData)
{
    data->view.reset(sf::FloatRect(0, 0, 2400, 1720));

}

MiniMap::~MiniMap()
{
}

void MiniMap::setArea(const sf::FloatRect &area)
{
    data->view.reset(area);
}

void MiniMap::setViewport(const sf::FloatRect &viewport)
{
    data->view.setViewport(viewport);
}

void MiniMap::draw(std::shared_ptr<Object> object)
{
    if(!object)
        return;

    auto app = Application::getInstance();
    auto window = app->getWindow();
    window->setView(data->view);

    window->draw(*object);

    auto camera = app->getComponent<Camera>();
    if(camera) {
        auto view2 = camera->getView();
        window->setView(view2);
    }
}

void MiniMap::update(float deltaTime)
{
    (void)deltaTime;
}
