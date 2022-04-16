#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Entity.h>
#include <Text.h>
#include <TileMap.h>

using namespace std;

int main()
{
    auto size = sf::Vector2f(800, 600);
    auto setting = sf::ContextSettings();
    setting.antialiasingLevel = 12;
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Chapter-3",
                  sf::Style::Close, setting);
    window->setVerticalSyncEnabled(true);

    auto app = Application::getInstance();
    app->setBackgroundColor(sf::Color::Blue);
    app->setWindow(window);

    auto tileMap = TileMap::createTileMap(TileMapType_Hex);
    tileMap->init(48, 36, 30);
    tileMap->setTextVisible(true);

    auto list = tileMap->getAdjacentTileByPosition(8, 5);
    for (auto itr = list.begin(); itr != list.end(); itr++)
        tileMap->getTileByIndex(itr->x, itr->y)->setVisible(false);

    app->execute(tileMap);
    return 0;
}


