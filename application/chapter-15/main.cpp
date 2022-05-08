#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Entity.h>
#include <Text.h>
#include <Scene.h>
#include <Sprite.h>
#include <SpriteDecorator.h>

using namespace std;

std::shared_ptr<Sprite> createSprite(const std::string &image, float x, float y)
{
    auto sprite = std::make_shared<Sprite>();
    sprite->setSpriteColor(sf::Color::Yellow);
    auto texture = Application::getInstance()->loadTexture(image);
    sprite->addTexture(*texture);

    auto spriteRing = std::make_shared<SpriteRingDecorator>();
    //spriteRing->setScaleFactor(1.0f);
    spriteRing->setFillColor(sf::Color::Red);
    sprite->setDecorator(spriteRing);

    sprite->setPosition(x, y);
    return sprite;
}

class SpriteMessageListener : public MessageListener
{
    // MessageListener interface
public:
    SpriteMessageListener(std::shared_ptr<Sprite> inputSprite):
        sprite(inputSprite)
    {
    }
public:
    bool onListener(std::shared_ptr<Message> message) override
    {
        auto sfml = std::dynamic_pointer_cast<SFMLMessage>(message);
        auto event = sfml->getEvent();
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Key::Left) {
                if(sprite->getPosition().x > 5)
                    sprite->move(-5, 0);
                return true;
            } else if (event.key.code == sf::Keyboard::Key::Right) {
                if(sprite->getPosition().x < Application::getInstance()->getWindow()->getSize().x - 5)
                    sprite->move(5, 0);
                return true;
            } else if (event.key.code == sf::Keyboard::Key::Up) {
                if(sprite->getPosition().y > 5)
                    sprite->move(0, -5);
                return true;
            } else if (event.key.code == sf::Keyboard::Key::Down) {
                if(sprite->getPosition().y < Application::getInstance()->getWindow()->getSize().y - 5)
                    sprite->move(0, 5);
                return true;
            }
        }

        return false;
    }
private:
    std::shared_ptr<Sprite> sprite;
};

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
    auto size = sf::Vector2f(800, 640);
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Chapter-15",
                  sf::Style::Close, settings);
    window->setVerticalSyncEnabled(true);

    auto app = Application::getInstance();
    app->setBackgroundColor(sf::Color::Black);
    app->setWindow(window);

    auto scene = std::make_shared<Scene>();
    scene->setName("scene");

    auto background = Application::getInstance()->loadTexture("../resource/images/background.png");
    scene->setBackground(*background);

    auto sprite = createSprite("../resource/images/plane.png", 400, 320);

    scene->addMessageListener(std::make_shared<SpriteMessageListener>(sprite));
    scene->addChild(sprite);

    auto font = std::make_shared<sf::Font>();
    font->loadFromFile("../resource/FZYTK.TTF");

    auto text = createText(font);
    text->setText(L"精灵装饰器", false);
    text->setPosition(80, 30);
    scene->addChild(text);

    app->execute(scene);

    return 0;
}
