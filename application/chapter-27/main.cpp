#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Text.h>
#include <Scene.h>
#include <SoundManager.h>
#include <Widget/Desktop.h>
#include <Widget/BoxLayout.h>
#include <Widget/Button.h>
#include <Widget/ProgressBar.h>

using namespace std;

int main()
{
    auto size = sf::Vector2f(1152, 720);
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

    auto box = std::make_shared<VBoxLayout>();

    for(int i = 0; i < 5; i++) {
        auto progressBar = std::make_shared<ProgressBar>();
        progressBar->setSize(sf::Vector2f(720, 18.0));
        progressBar->setValue(i * 20);
        box->addWidget(progressBar);
    }

    desktop->addWidget(box, HMode_Center, VMode_Center);

    app->execute(scene);
    return 0;
}
