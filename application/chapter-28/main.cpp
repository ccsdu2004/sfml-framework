#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <Application.h>
#include <Scene.h>
#include <Charts/RadarChart.h>

using namespace std;

int main()
{
    auto size = sf::Vector2f(960, 760);
    auto setting = sf::ContextSettings();
    setting.antialiasingLevel = 12;
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Chapter-28",
                                                     sf::Style::Close, setting);
    window->setVerticalSyncEnabled(true);

    auto app = Application::getInstance();
    app->setBackgroundColor(sf::Color::Black);
    app->setWindow(window);

    auto radar = std::make_shared<RadarChart>();
    radar->addData(0, 60, sf::Color::Blue);
    radar->addData(45, 120);
    radar->addData(90, 90);
    radar->addData(120, 72, sf::Color::Green);
    radar->autoRange();

    radar->setSize(300, 300);
    radar->setCenter(sf::Vector2f(480, 380));
    radar->setBackgroundColor(sf::Color::Transparent);

    app->execute(radar);
    return 0;
}


