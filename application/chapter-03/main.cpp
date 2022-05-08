#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Entity.h>
#include <TileMap.h>
#include <Text.h>

using namespace std;

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
    tileMap->init(23, 15, 24);
    tileMap->setTextVisible(true);

    auto list = tileMap->getAdjacentTileByPosition(8, 5);
    for (auto itr = list.begin(); itr != list.end(); itr++)
        tileMap->getTileByIndex(itr->x, itr->y)->setVisible(false);

    auto font = std::make_shared<sf::Font>();
    font->loadFromFile("../resource/FZYTK.TTF");

    auto text = createText(font);
    text->setText(L"六边地图", false);
    text->setPosition(80, 30);
    tileMap->addChild(text);

    app->execute(tileMap);
    return 0;
}


