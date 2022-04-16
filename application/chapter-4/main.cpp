#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Entity.h>
#include <Text.h>
#include <Sprite.h>
#include <TileMap.h>

using namespace std;

std::shared_ptr<TileMap> tileMap;

std::shared_ptr<Object> createSprite(const std::string &image, float x, float y)
{
    auto sprite = std::make_shared<Sprite>();
    sprite->setBackgroundColor(sf::Color::Red);
    sprite->setPosition(x, y);
    auto texture = Application::getInstance()->loadTexture(image);
    sprite->setTexture(*texture);
    return sprite;
}

void clickedTile(int32_t x, int32_t y)
{
    std::cout << "clicked tile:" << x << "," << y << std::endl;
    tileMap->getTileByIndex(x, y)->setFillColor(sf::Color::Red);
}

int main()
{
    auto size = sf::Vector2f(800, 600);
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Chapter-4",
                  sf::Style::Close);
    window->setVerticalSyncEnabled(true);

    auto app = Application::getInstance();
    app->setBackgroundColor(sf::Color::Black);
    app->setWindow(window);

    auto object = std::make_shared<Object>();

    tileMap = TileMap::createTileMap(TileMapType_Hex);
    tileMap->tileClicked.connect(clickedTile);
    tileMap->init(48, 36, 32);
    tileMap->setMessageReceived(true);
    tileMap->setTextVisible(false);

    object->addChild(tileMap);

    object->addChild(createSprite("../resource/icon/nato/units/infantry.png", 10, 10));
    object->addChild(createSprite("../resource/icon/nato/units/aviation_fr.png", 80, 10));
    object->addChild(createSprite("../resource/icon/nato/units/armour.png", 150, 10));

    app->execute(object);
    return 0;
}

