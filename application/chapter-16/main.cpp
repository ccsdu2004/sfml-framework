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

    auto spriteFanShapeDecorator = std::make_shared<SpriteFanShapeDecorator>();
    spriteFanShapeDecorator->setRadius(96.0f);
    spriteFanShapeDecorator->setFillColor(sf::Color::Cyan);
    spriteFanShapeDecorator->setIncludedAngle(90.0f);
    sprite->setDecorator(spriteFanShapeDecorator);

    sprite->setPosition(x, y);
    auto texture = Application::getInstance()->loadTexture(image);
    sprite->addTexture(*texture);
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
                if (sprite->getPosition().x > 5)
                    sprite->move(-5, 0);
                return true;
            } else if (event.key.code == sf::Keyboard::Key::Right) {
                if (sprite->getPosition().x < Application::getInstance()->getWindow()->getSize().x - 5)
                    sprite->move(5, 0);
                return true;
            } else if (event.key.code == sf::Keyboard::Key::Up) {
                if (sprite->getPosition().y > 5)
                    sprite->move(0, -5);
                return true;
            } else if (event.key.code == sf::Keyboard::Key::Down) {
                if (sprite->getPosition().y < Application::getInstance()->getWindow()->getSize().y - 5)
                    sprite->move(0, 5);
                return true;
            } else if (event.key.code == sf::Keyboard::Key::Space) {
                sprite->setRotate(sprite->getRotate() + 15);
                return true;
            }
        }

        return false;
    }
private:
    std::shared_ptr<Sprite> sprite;
};

int main()
{
    auto size = sf::Vector2f(800, 640);
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Chapter-16",
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

    auto text = scene->createToastText();
    text->setText(L"精灵视野", false);
    text->setPosition(80, 30);
    scene->addChild(text);

    app->execute(scene);

    return 0;
}

