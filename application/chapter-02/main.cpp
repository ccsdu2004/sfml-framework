#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
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

    for(int i = 0; i < 100; i++) {
        auto text = createText(font);
        text->setPosition(rand() % 800, rand() % 600);
        text->setText(std::string("Text Object:") + std::to_string(i), rand() % 2 == 0);
        text->setCharacterSize(rand() % 8 + 12);
        text->setOutlineColor(sf::Color::White);
        text->setTextColor(sf::Color(rand() % 255, rand() % 255, rand() % 255, 255));
        text->setOutlineThickness(1.2f);
        text->setStyle(sf::Text::Style(rand() % 8));
        object->addChild(text);
    }

    app->execute(object);
    return 0;
}

std::shared_ptr<Text> createText(std::shared_ptr<sf::Font> font)
{
    auto text = std::make_shared<Text>();
    text->setFont(font);
    text->setCharacterSize(15);
    text->setTextColor(sf::Color::White);
    text->setSize(150, 48);
    text->setBackgroundColor(sf::Color::Black);
    return text;
}

