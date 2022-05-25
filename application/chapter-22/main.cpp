#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Text.h>
#include <Scene.h>
#include <Widget/Desktop.h>
#include <Widget/Widget.h>
#include <Widget/BoxLayout.h>
#include <Widget/MouseArea.h>

using namespace std;

int main()
{
    auto size = sf::Vector2f(800, 600);
    auto setting = sf::ContextSettings();
    setting.antialiasingLevel = 12;
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Chapter-22",
                                                     sf::Style::Close, setting);
    window->setVerticalSyncEnabled(true);

    auto app = Application::getInstance();
    app->setBackgroundColor(sf::Color::Blue);
    app->setWindow(window);

    auto scene = std::make_shared<Scene>();
    auto desktop = std::make_shared<Desktop>();
    scene->addComponent(desktop);

    auto boxLayout = std::make_shared<HBoxLayout>();
    boxLayout->setSpacing(3.0f);
    boxLayout->setMargin(3.0f);
    desktop->addWidget(boxLayout, HMode_Center, VMode_Center);

    for (int i = 0; i < 3; i++) {
        auto mouseArea = std::make_shared<MouseArea>(sf::Vector2f(80.0f, 240.0f));
        if (i == 0)
            boxLayout->addWidget(mouseArea, VMode_Top);
        else if (i == 1)
            boxLayout->addWidget(mouseArea, VMode_Center);
        else
            boxLayout->addWidget(mouseArea, VMode_Bottom);
        mouseArea->setBackgroundColor(sf::Color::Transparent);
        mouseArea->setOutlineColor(sf::Color::White);
        mouseArea->setOutlineThickness(1.0f);
        mouseArea->setTargetWidget(boxLayout);
    }

    boxLayout->setBackgroundColor(sf::Color::Black);

    auto text = scene->createToastText();
    text->setText(L"控件布局", false);
    text->setPosition(80, 30);
    scene->addChild(text);

    app->execute(scene);
    return 0;
}

