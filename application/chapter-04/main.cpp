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
    sprite->addTexture(*texture);
    return sprite;
}

std::shared_ptr<Text> createText(std::shared_ptr<sf::Font> font)
{
    auto text = std::make_shared<Text>();
    text->setFont(font);
    text->setCharacterSize(18);
    text->setTextColor(sf::Color::White);
    text->setSize(120, 36);
    text->setBackgroundColor(sf::Color::Black);

    text->setOutlineColor(sf::Color::Yellow);
    text->setOutlineThickness(0.6f);
    return text;
}

void clickedTile(int32_t x, int32_t y)
{
    auto tile = tileMap->getTileByIndex(x,y);
    if(tile)
        tile->setFillColor(sf::Color::Red);
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

    std::vector<std::string> units = {
        "./../resource/icon/nato/units/air_defence.png",
        "./../resource/icon/nato/units/ammunition.png",
        "./../resource/icon/nato/units/antitank.png",
        "./../resource/icon/nato/units/armour.png",
        "./../resource/icon/nato/units/aviation_fr.png",
        "./../resource/icon/nato/units/aviation_rot.png",
        "./../resource/icon/nato/units/bridge.png",
        "./../resource/icon/nato/units/cbrn.png",
        "./../resource/icon/nato/units/combatservicesupport.png",
        "./../resource/icon/nato/units/electronicwarfare.png",
        "./../resource/icon/nato/units/engenier.png",
        "./../resource/icon/nato/units/expliosive.png"
    };

    std::vector<std::string> sizes = {
        "./../resource/icon/nato/sizes/army.png",
        "./../resource/icon/nato/sizes/army_group.png",
        "./../resource/icon/nato/sizes/battalion.png",
        "./../resource/icon/nato/sizes/brigade.png",
        "./../resource/icon/nato/sizes/company.png",
        "./../resource/icon/nato/sizes/corps.png",
        "./../resource/icon/nato/sizes/crew.png",
        "./../resource/icon/nato/sizes/division.png",
        "./../resource/icon/nato/sizes/fireteam.png",
        "./../resource/icon/nato/sizes/group.png",
        "./../resource/icon/nato/sizes/maneuver_team.png",
        "./../resource/icon/nato/sizes/platoon.png",
        "./../resource/icon/nato/sizes/region.png",
        "./../resource/icon/nato/sizes/section.png"
    };

    std::vector<std::string> modifers = {
        "./../resource/icon/nato/modifiers/airborne.png",
        "./../resource/icon/nato/modifiers/airmobile.png",
        "./../resource/icon/nato/modifiers/airmobile_ol.png",
        "./../resource/icon/nato/modifiers/amphibious.png",
        "./../resource/icon/nato/modifiers/motorized.png",
        "./../resource/icon/nato/modifiers/mountain.png",
        "./../resource/icon/nato/modifiers/paratrooper.png",
        "./../resource/icon/nato/modifiers/rocket.png",
        "./../resource/icon/nato/modifiers/wheeled.png"
    };

    std::vector<sf::Color> spriteColor = {sf::Color::White, sf::Color::Red, sf::Color::Green, sf::Color::Blue};

    for(int i = 0; i < 30; i++) {
        auto sprite = createSprite(units.at(rand() % units.size()), rand() % 800, rand() % 600);
        int index = rand() % modifers.size();
        sprite->addTexture(*Application::getInstance()->loadTexture(modifers[index]));
        index = rand() % sizes.size();
        sprite->addTexture(*Application::getInstance()->loadTexture(sizes[index]));
        sprite->setSpriteColor(spriteColor[rand() % spriteColor.size()]);
        object->addChild(sprite);
    }

    auto font = std::make_shared<sf::Font>();
    font->loadFromFile("../resource/FZYTK.TTF");

    auto text = createText(font);
    text->setText(L"精灵", false);
    text->setPosition(80, 30);
    object->addChild(text);

    app->execute(object);
    return 0;
}

