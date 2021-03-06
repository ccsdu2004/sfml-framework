#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Entity.h>
#include <Text.h>
#include <Util.h>
#include <Scene.h>
#include <Sprite.h>
#include <Animation.h>
#include <MovingSprite.h>
#include <QuadTreeScene.h>
#include <SpriteDeleter.h>
#include <ResourceManager.h>

auto screenSize = sf::Vector2f(960, 640);
#define BULLET_SPEED sf::Vector2f(0, -160.0f)

using namespace std;

std::shared_ptr<QuadTreeScene> scene;

std::shared_ptr<Sprite> createSprite(const std::string &image, VMode vMode)
{
    auto sprite = std::make_shared<Sprite>();
    auto textureManager = Application::getInstance()->getComponent<ResourceManager<sf::Texture>>();

    auto texture = textureManager->loadFromFile(image);
    sprite->addTexture(*texture);

    sf::Vector2f size(texture->getSize().x, texture->getSize().y);
    auto position = Entity::adjustPosition(sf::FloatRect(sf::Vector2f(), screenSize), size,
                                           HMode_Center, vMode);
    sprite->setPosition(position);
    return sprite;
}

class MyQuadTreeScene : public QuadTreeScene
{
public:
    void onConllision(SpritePointer current, SpritePointer target) override
    {
        current->setSpriteStatus(SpriteStatus_Death);
        sf::FloatRect area;
        if (current->getBoundingBox().intersects(target->getBoundingBox(), area)) {
            auto position = getRectCenter(area);
            auto animation = createAnimation();
            animation->setPosition(position);
            addChild(animation);
        }
    }
private:
    std::shared_ptr<Animation> createAnimation()
    {
        std::vector<sf::IntRect> areas;

        for (int i = 0; i < 6; i++) {
            auto area = sf::IntRect(i * 85, 0, 85, 85);
            areas.push_back(area);
        }

        std::shared_ptr<Animation> animation = std::make_shared<Animation>();
        animation->setSize(85, 85);
        animation->setDurationPerFrame(0.2f);
        animation->setSingleShot(true);

        animation->setBackgroundColor(sf::Color::White);
        animation->setTexture("../resource/images/blast2.png", areas);
        animation->setCurrentItem(0);
        animation->start();
        return animation;
    }
};

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
                if (sprite->getPosition().x + sprite->getSize().x <
                    Application::getInstance()->getWindow()->getSize().x - 5)
                    sprite->move(5, 0);
                return true;
            } else if (event.key.code == sf::Keyboard::Key::Space) {
                auto textureManager = Application::getInstance()->getComponent<ResourceManager<sf::Texture>>();
                auto texture = textureManager->loadFromFile("../resource/images/bullet.png");
                auto bullet = std::make_shared<MovingSprite>();
                bullet->setSpriteStatus(SpriteStatus_Normal);
                bullet->setSpriteOwner(sprite);
                bullet->addTexture(*texture);

                auto position = sprite->getCenter();
                position.y -= sprite->getSize().y * 0.5f;

                bullet->setCenter(position);
                bullet->setVelocity(BULLET_SPEED);
                bullet->setSpriteGroup(SpriteGroupID_Bullet);
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
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(screenSize.x, screenSize.y),
                  "Chapter-9",
                  sf::Style::Close);
    window->setVerticalSyncEnabled(true);

    auto app = Application::getInstance();
    app->setBackgroundColor(sf::Color::Black);
    app->setWindow(window);

    scene = std::make_shared<MyQuadTreeScene>();
    scene->setName("scene");

    scene->addConllisionGroupID(SpriteGroupID_Bullet);
    scene->addConllisionGroupID(SpriteGroupID_PlayerA);
    scene->addConllisionGroupID(SpriteGroupID_PlayerB);

    auto spriteDeleter = SpriteDeleter::create(SpriteDeleter_Slop);
    scene->addSpriteDeleter(SpriteGroupID_Bullet, spriteDeleter);

    auto textureManager = Application::getInstance()->getComponent<ResourceManager<sf::Texture>>();
    auto background = textureManager->loadFromFile("../resource/images/background.png");
    scene->setBackground(*background);

    auto sprite = createSprite("../resource/images/plane.png", VMode_Bottom);
    sprite->setSpriteGroup(SpriteGroupID_PlayerB);
    scene->addMessageListener(std::make_shared<SpriteMessageListener>(sprite));
    scene->addChild(sprite);

    auto enemy = createSprite("../resource/images/enemy1.png", VMode_Top);
    enemy->setSpriteGroup(SpriteGroupID_PlayerB);
    scene->addChild(enemy);

    auto font = std::make_shared<sf::Font>();
    font->loadFromFile("../resource/FZYTK.TTF");

    auto text = scene->createToastText();
    text->setText(L"???????????????", false);
    text->setPosition(30, 30);
    scene->addChild(text);

    auto sceneManager = std::make_shared<SceneManager>();
    sceneManager->setInitialScene(scene);
    app->execute(sceneManager);

    return 0;
}
