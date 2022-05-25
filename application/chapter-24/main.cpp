#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Text.h>
#include <Scene.h>
#include <Widget/Desktop.h>
#include <Widget/BoxLayout.h>
#include <Widget/BasicButton.h>

using namespace std;

int main()
{
    auto size = sf::Vector2f(800, 600);
    auto setting = sf::ContextSettings();
    setting.antialiasingLevel = 12;
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Chapter-24",
                  sf::Style::Close, setting);
    window->setVerticalSyncEnabled(true);

    auto app = Application::getInstance();
    app->setBackgroundColor(sf::Color::Blue);
    app->setWindow(window);

    auto scene = std::make_shared<Scene>();
    auto desktop = std::make_shared<Desktop>();
    scene->addComponent(desktop);

    auto boxLayout = std::make_shared<VBoxLayout>();
    boxLayout->setSpacing(3.0f);
    boxLayout->setMargin(3.0f);
    desktop->addWidget(boxLayout, HMode_Center, VMode_Center);

    for (int i = 0; i < 3; i++) {
        auto button = std::make_shared<BasicButton>();
        button->setSize(200, 60);
        boxLayout->addWidget(button, HMode_Center);
    }

    boxLayout->setBackgroundColor(sf::Color::Black);

    auto text = scene->createToastText();
    text->setText(L"BasicButton", false);
    text->setPosition(80, 30);
    scene->addChild(text);

    app->execute(scene);
    return 0;
}

