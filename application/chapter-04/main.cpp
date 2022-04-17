#include <iostream>
#include <array>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Entity.h>
#include <Text.h>
#include <Sprite.h>
#include <TileMap.h>

using namespace std;

std::shared_ptr<TileMap> tileMap;

std::shared_ptr<Sprite> createSprite(const std::string &image, float x, float y)
{
    auto sprite = std::make_shared<Sprite>();
    sprite->setPosition(x, y);
    auto texture = Application::getInstance()->loadTexture(image);
    sprite->setTexture(*texture);
    return sprite;
}

void clickedTile(int32_t x, int32_t y)
{
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
    tileMap->setMessageReceived(false);
    tileMap->setTextVisible(false);

    object->addChild(tileMap);

    std::array<std::string, 8> icons = {"antitank.png", "air_defence.png", "antitank.png", "armour.png", "aviation_fr.png", "aviation_rot.pn", "cbrn.png", "hospital.png"};

    for(int i = 0; i < 300; i++) {
        std::string image("../resource/icon/nato/units/");
        int index = rand() % 8;
        image += icons.at(index);
        auto sprite = createSprite(image, rand() % 800, rand() % 600);
        sprite->setSpriteColor(sf::Color(rand() % 255, rand() % 255, rand() % 255, 255));
        object->addChild(sprite);
    }

    app->execute(object);
    return 0;
}

