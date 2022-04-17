#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Entity.h>
#include <Text.h>
#include <Scene.h>
#include <Sprite.h>

using namespace std;

std::shared_ptr<Object> createSprite(const std::string &image, float x, float y)
{
    auto sprite = std::make_shared<Sprite>();
    sprite->setBackgroundColor(sf::Color::Red);
    sprite->setPosition(x, y);
    auto texture = Application::getInstance()->loadTexture(image);
    sprite->setTexture(*texture);
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
            if (event.key.code == sf::Keyboard::Key::A) {
                if(sprite->getPosition().x > 5)
                    sprite->move(-5, 0);
                return true;
            } else if (event.key.code == sf::Keyboard::Key::D) {
                if(sprite->getPosition().x + sprite->getSize().x < Application::getInstance()->getWindow()->getSize().x - 5)
                    sprite->move(5, 0);
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
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Chapter-5",
                  sf::Style::Close);
    window->setVerticalSyncEnabled(true);

    auto app = Application::getInstance();
    app->setBackgroundColor(sf::Color::Black);
    app->setWindow(window);

    auto scene = std::make_shared<Scene>();
    scene->setName("scene");

    auto background = Application::getInstance()->loadTexture("../resource/images/background.png");
    scene->setBackground(*background);

    auto sprite = createSprite("../resource/images/plane.png", 400, 320);
    //scene->addMessageListener(std::make_shared<SpriteMessageListener>(sprite));
    scene->addChild(sprite);

    /*auto sceneManager = std::make_shared<SceneManager>();
    sceneManager->setInitialScene(scene);
    sceneManager->addChild(sprite);
    app->execute(sceneManager);*/

    auto object = std::make_shared<Object>();
    object->addChild(sprite);
    app->execute(object);

    return 0;
}

