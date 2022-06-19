#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Entity.h>
#include <NameHolder.h>

using namespace std;

std::vector<EntityPointer> units;

class Unit : public Entity, public NameHolder
{
public:
    Unit(const sf::Vector2f &size = sf::Vector2f()):
        Entity(size, CornerStyle())
    {
        setOutlineColor(sf::Color::Yellow);
        setOutlineThickness(1.0f);
        setBackgroundColor(sf::Color(rand() % 250, rand() % 250, rand() % 250));
    }
private:
    void onMouseEnter()
    {
        setOutlineColor(sf::Color::Red);
    }

    void onMouseExit()
    {
        setOutlineColor(sf::Color::Green);
    }

    void onMousePressed(sf::Mouse::Button button)
    {
        (void)button;
        setOutlineColor(sf::Color::White);
    }

    void onMouseReleased(sf::Mouse::Button button)
    {
        (void)button;
        setOutlineColor(sf::Color::Black);

        for (auto unit : units)
            unit->setRotate(unit->getRotate() + 15.0f);
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
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 600), "Chapter-1",
                  sf::Style::Close);
    window->setVerticalSyncEnabled(true);

    auto app = Application::getInstance();
    app->setBackgroundColor(sf::Color::Blue);
    app->setWindow(window);

    auto object = std::make_shared<Object>();

    int xoffset = 0;
    int yoffset = 0;

    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 18; j++) {
            auto item = std::make_shared<Unit>(sf::Vector2f(100, 36));
            item->setName(std::to_string(i) + "," + std::to_string(j));
            units.push_back(std::dynamic_pointer_cast<Entity>(item));
            item->setPosition(xoffset + i * 120, yoffset + j * 40);
            object->addChild(item);
        }

    app->execute(object);
    return 0;
}
