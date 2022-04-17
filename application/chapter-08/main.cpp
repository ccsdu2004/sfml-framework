#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Entity.h>
#include <Text.h>
#include <Scene.h>
#include <Sprite.h>
#include <MovingSprite.h>
#include <BattleScene.h>
#include <SpriteGroup.h>

#define BULLET_SPEED sf::Vector2f(0, -160.0f)

using namespace std;

std::shared_ptr<BattleScene> scene;

std::shared_ptr<Sprite> createSprite(const std::string &image, float x, float y)
{
    auto sprite = std::make_shared<Sprite>();
    sprite->setPosition(x, y);
    auto texture = Application::getInstance()->loadTexture(image);
    sprite->setTexture(*texture);
    auto size = texture->getSize();
    sprite->setSize(size.x, size.y);
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
            } else if (event.key.code == sf::Keyboard::Key::Space) {
                auto texture = Application::getInstance()->loadTexture("../resource/images/bullet.png");
                auto bullet = std::make_shared<MovingSprite>();
                bullet->setTexture(*texture);

                auto position = sprite->getCenter();
                position.x -= bullet->getSize().x * 0.5f;
                position.y -= sprite->getSize().y;

                bullet->setPosition(position);
                bullet->setSpeed(BULLET_SPEED);
                scene->addSpriteToGroup(bullet, SpriteGroupID_Bullet);
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
    auto size = sf::Vector2f(960, 640);
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Chapter-8",
                  sf::Style::Close);
    window->setVerticalSyncEnabled(true);

    auto app = Application::getInstance();
    app->setBackgroundColor(sf::Color::Black);
    app->setWindow(window);

    scene = std::make_shared<BattleScene>();
    scene->setName("scene");

    auto background = Application::getInstance()->loadTexture("../resource/images/background.png");
    scene->setBackground(*background);

    auto sprite = createSprite("../resource/images/plane.png", 240, 360);
    scene->addMessageListener(std::make_shared<SpriteMessageListener>(sprite));
    scene->addSpriteToGroup(sprite, SpriteGroupID_PlayerA);

    auto enemy = createSprite("../resource/images/enemy1.png", 240, 24);
    scene->addSpriteToGroup(enemy, SpriteGroupID_PlayerB);

    auto sceneManager = std::make_shared<SceneManager>();
    sceneManager->setInitialScene(scene);
    app->execute(sceneManager);

    return 0;
}
