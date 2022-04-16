#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Entity.h>
#include <Scene.h>
#include <Sprite.h>
#include <Switch.h>
#include <Animation.h>

using namespace std;

int main()
{
    auto size = sf::Vector2f(960, 640);
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Chapter-6",
                  sf::Style::Close);
    window->setVerticalSyncEnabled(true);

    auto app = Application::getInstance();
    app->setBackgroundColor(sf::Color::Black);
    app->setWindow(window);

    auto scene = std::make_shared<Scene>();
    scene->setName("scene");

    auto background = Application::getInstance()->loadTexture("../resource/images/background.png");
    scene->setBackground(*background);

    std::vector<sf::IntRect> areas;

    for(int i = 0; i < 6; i++) {
        auto area = sf::IntRect(i * 85, 0, 85, 85);
        areas.push_back(area);
    }

    std::shared_ptr<Animation> animation = std::make_shared<Animation>();
    animation->setDurationPerFrame(120.0f);
    animation->setSingleShot(false);
    animation->setPosition(size.x * 0.5 - 32, size.y * 0.5 - 32);

    animation->setTexture("../resource/images/blast2.png", areas);

    scene->addChild(animation);

    auto sceneManager = std::make_shared<SceneManager>();
    sceneManager->setInitialScene(scene);
    app->execute(sceneManager);

    return 0;
}

