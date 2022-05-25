#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Text.h>
#include <Scene.h>
#include <Widget/Desktop.h>
#include <Widget/Label.h>

using namespace std;

int main()
{
    auto size = sf::Vector2f(800, 600);
    auto setting = sf::ContextSettings();
    setting.antialiasingLevel = 12;
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Chapter-23",
                                                     sf::Style::Close, setting);
    window->setVerticalSyncEnabled(true);

    auto app = Application::getInstance();
    app->setBackgroundColor(sf::Color::Blue);
    app->setWindow(window);

    auto scene = std::make_shared<Scene>();
    auto desktop = std::make_shared<Desktop>();
    scene->addComponent(desktop);

    auto label = std::make_shared<Label>();
    label->setSize(200, 60);
    label->setText(L"展示一个标签");

    auto style = std::make_shared<LabelStyle>();
    style->textColor = sf::Color::Red;
    label->setWidgetStyle(style);

    desktop->addWidget(label, HMode_Center, VMode_Top);

    auto text = scene->createToastText();
    text->setText(L"文本标签", false);
    text->setPosition(80, 30);
    scene->addChild(text);

    app->execute(scene);
    return 0;
}

