#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Entity.h>
#include <TileMap.h>
#include <Text.h>
#include <Scene.h>
#include <ShortestPathFinder.h>
#include <Widget/Desktop.h>
#include <Widget/Widget.h>
#include <Widget/MouseArea.h>

using namespace std;

int main()
{
    auto size = sf::Vector2f(800, 600);
    auto setting = sf::ContextSettings();
    setting.antialiasingLevel = 12;
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Chapter-20",
                  sf::Style::Close, setting);
    window->setVerticalSyncEnabled(true);

    auto app = Application::getInstance();
    app->setBackgroundColor(sf::Color::Blue);
    app->setWindow(window);

    auto scene = std::make_shared<Scene>();
    auto desktop = std::make_shared<Desktop>();
    scene->addComponent(desktop);

    auto mouseArea = std::make_shared<MouseArea>(sf::Vector2f(240.0f, 180.0f));
    desktop->addWidget(mouseArea, HMode_Center, VMode_Center);
    mouseArea->setBackgroundColor(sf::Color::Transparent);
    mouseArea->setOutlineColor(sf::Color::White);
    mouseArea->setOutlineThickness(1.0f);
    mouseArea->setTargetWidget(mouseArea);

    auto text = scene->createToastText();
    text->setText(L"鼠标拖拽区域", false);
    text->setPosition(80, 30);
    scene->addChild(text);

    app->execute(scene);
    return 0;
}

