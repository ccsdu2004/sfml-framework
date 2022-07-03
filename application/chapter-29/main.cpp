#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Entity.h>
#include <NameHolder.h>

using namespace std;

class Unit : public Entity, public NameHolder
{
public:
    Unit(const sf::Vector2f &size = sf::Vector2f()):
        Entity(size)
    {
        setOutlineColor(sf::Color::Yellow);
        setOutlineThickness(1.0f);
        setBackgroundColor(sf::Color::Green);
    }
private:
    void onMouseEnter()
    {
        setBackgroundColor(sf::Color::Red);
    }

    void onMouseExit()
    {
        setBackgroundColor(sf::Color::Green);
    }

    void onMousePressed(sf::Mouse::Button button)
    {
        (void)button;
        setBackgroundColor(sf::Color::White);
    }

    void onMouseReleased(sf::Mouse::Button button)
    {
        (void)button;
        setBackgroundColor(sf::Color::Black);
    }

    void onMouseWheelScroll(float scroll)
    {
        (void)scroll;
    }

    void onMouseMoved(int x, int y)
    {
        (void)x, (void)y;
    }
};

int main()
{
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(600, 600), "Chapter-29",
                  sf::Style::Close);
    window->setVerticalSyncEnabled(true);

    auto app = Application::getInstance();
    app->setBackgroundColor(sf::Color::Blue);
    app->setWindow(window);

    auto parent = std::make_shared<Unit>();
    parent->setName("parent");
    parent->setSize(300, 300);
    parent->setPosition(150, 150);
    parent->setBackgroundColor(sf::Color::Black);

    auto object = std::make_shared<Unit>();
    object->setName("object");
    object->setSize(200, 200);
    object->setPosition(100, 100);
    object->setBackgroundColor(sf::Color::Blue);

    parent->addChild(object);

    auto child = std::make_shared<Unit>();
    child->setName("child");
    child->setSize(100, 100);
    child->setPosition(100, 100);
    child->setBackgroundColor(sf::Color::Yellow);
    object->addChild(child);

    app->execute(parent);
    return 0;
}
