#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Entity.h>
#include <Text.h>

using namespace std;

std::shared_ptr<Text> createText(std::shared_ptr<sf::Font> font);

int main()
{
    auto size = sf::Vector2f(800, 600);
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Chapter-2",
                  sf::Style::Close);
    window->setVerticalSyncEnabled(true);

    auto app = Application::getInstance();
    app->setBackgroundColor(sf::Color::Blue);
    app->setWindow(window);

    auto object = std::make_shared<Object>();

    auto font = std::make_shared<sf::Font>();
    font->loadFromFile("../resource/FZYTK.TTF");

    auto text = createText(font);
    text->setPosition(400, 48);
    text->setText(L"这是一个文本对象", true);
    object->addChild(text);

    text = createText(font);
    text->setStyle(sf::Text::Style::Regular);
    text->setPosition(400, 84);
    text->setCharacterSize(16);
    text->setText(L"又是一个文本对象", false);
    object->addChild(text);

    app->execute(object);
    return 0;
}

std::shared_ptr<Text> createText(std::shared_ptr<sf::Font> font)
{
    auto text = std::make_shared<Text>();
    text->setFont(font);
    text->setCharacterSize(13);
    text->setTextColor(sf::Color::White);
    text->setSize(150, 32);
    text->setOutlineColor(sf::Color::Cyan);
    text->setOutlineThickness(1.0f);
    text->setBackgroundColor(sf::Color::Black);
    return text;
}

