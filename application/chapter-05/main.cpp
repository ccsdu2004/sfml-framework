#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Entity.h>
#include <Text.h>
#include <Scene.h>
#include <Sprite.h>
#include <ResourceManager.h>

auto screenSize = sf::Vector2f(800, 640);

using namespace std;

std::shared_ptr<Sprite> createSprite(const std::string &image)
{
    auto sprite = std::make_shared<Sprite>();
    sprite->setSpriteColor(sf::Color::Yellow);

    auto textureManager = Application::getInstance()->getComponent<ResourceManager<sf::Texture>>();

    auto texture = textureManager->loadFromFile(image);
    sprite->addTexture(*texture);

    sf::Vector2f size(texture->getSize().x, texture->getSize().y);
    auto position = Entity::adjustPosition(sf::FloatRect(sf::Vector2f(), screenSize), size, HMode_Center, VMode_Center);
    sprite->setPosition(position);
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
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(screenSize.x, screenSize.y), "Chapter-5",
                  sf::Style::Close);
    window->setVerticalSyncEnabled(true);

    auto app = Application::getInstance();
    app->setBackgroundColor(sf::Color::Black);
    app->setWindow(window);

    auto scene = std::make_shared<Scene>();
    scene->setName("scene");

    auto textureManager = Application::getInstance()->getComponent<ResourceManager<sf::Texture>>();

    auto background = textureManager->loadFromFile("../resource/images/background.png");
    scene->setBackground(*background);

    auto sprite = createSprite("../resource/images/plane.png");
    scene->addMessageListener(std::make_shared<SpriteMessageListener>(sprite));
    scene->addChild(sprite);

    auto font = std::make_shared<sf::Font>();
    font->loadFromFile("../resource/FZYTK.TTF");

    auto text = createText(font);
    text->setText(L"消息监听", false);
    text->setPosition(30, 30);
    scene->addChild(text);

    app->execute(scene);

    return 0;
}

