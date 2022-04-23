#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Entity.h>
#include <Scene.h>
#include <QuadTree.h>

using namespace std;

int main()
{
    auto size = sf::Vector2f(860, 640);
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Chapter-8",
                  sf::Style::Close);
    window->setVerticalSyncEnabled(true);

    auto app = Application::getInstance();
    app->setBackgroundColor(sf::Color::Black);
    app->setWindow(window);

    auto scene = std::make_shared<Scene>();
    scene->setName("scene");

    auto background = Application::getInstance()->loadTexture("../resource/images/grid.png");
    scene->setBackground(*background);

    auto sceneManager = std::make_shared<SceneManager>();
    sceneManager->setInitialScene(scene);
    app->execute(sceneManager);

    return 0;
}
