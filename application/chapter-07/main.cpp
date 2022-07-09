#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Entity.h>
#include <Scene.h>
#include <Text.h>
#include <MovingSprite.h>
#include <Util.h>
#include <ResourceManager.h>

auto screenSize = sf::Vector2f(960, 640);
#define BULLET_SPEED sf::Vector2f(0, -160.0f)

using namespace std;

std::shared_ptr<Scene> scene;

std::shared_ptr<Sprite> createSprite(const std::string &image, VMode vMode)
{
    auto sprite = std::make_shared<Sprite>();
    auto textureManager = Application::getInstance()->getComponent<ResourceManager<sf::Texture>>();

    auto texture = textureManager->loadFromFile(image);
    sprite->addTexture(*texture);

    sf::Vector2f size(texture->getSize().x, texture->getSize().y);
    auto position = Entity::adjustPosition(sf::FloatRect(sf::Vector2f(), screenSize), size, HMode_Center, vMode);
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
            if (event.key.code == sf::Keyboard::Key::A) {
                if (sprite->getPosition().x > 5)
                    sprite->move(-5, 0);
                return true;
            } else if (event.key.code == sf::Keyboard::Key::D) {
                if (sprite->getPosition().x + sprite->getSize().x <
                    Application::getInstance()->getWindow()->getSize().x - 5)
                    sprite->move(5, 0);
                return true;
            } else if (event.key.code == sf::Keyboard::Key::Space) {
                auto textureManager = Application::getInstance()->getComponent<ResourceManager<sf::Texture>>();

                auto texture = textureManager->loadFromFile("../resource/images/bullet.png");
                auto bullet = std::make_shared<MovingSprite>();
                bullet->addTexture(*texture);

                auto position = getRectCenter(sprite->getBoundingBox());
                position.y -= sprite->getSize().y;

                bullet->setCenter(position);
                bullet->setVelocity(BULLET_SPEED);
                scene->addChild(bullet);
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
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(screenSize.x, screenSize.y), "Chapter-7",
                  sf::Style::Close);
    window->setVerticalSyncEnabled(true);

    auto app = Application::getInstance();
    app->setBackgroundColor(sf::Color::Black);
    app->setWindow(window);

    scene = std::make_shared<Scene>();
    scene->setName("scene");

    auto textureManager = Application::getInstance()->getComponent<ResourceManager<sf::Texture>>();
    auto background = textureManager->loadFromFile("../resource/images/background.png");
    scene->setBackground(*background);

    auto sprite = createSprite("../resource/images/plane.png", VMode_Bottom);
    scene->addMessageListener(std::make_shared<SpriteMessageListener>(sprite));
    scene->addChild(sprite);

    auto enemy = createSprite("../resource/images/enemy1.png", VMode_Top);
    scene->addChild(enemy);

    auto font = std::make_shared<sf::Font>();
    font->loadFromFile("../resource/FZYTK.TTF");

    auto text = scene->createToastText();
    text->setText(L"游戏场景", false);
    text->setPosition(30, 30);
    scene->addChild(text);

    auto sceneManager = std::make_shared<SceneManager>();
    sceneManager->setInitialScene(scene);
    app->execute(sceneManager);

    return 0;
}
