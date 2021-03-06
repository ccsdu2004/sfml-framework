#include <iostream>
#include <cmath>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Entity.h>
#include <Text.h>
#include <Scene.h>
#include <Sprite.h>
#include <Animation.h>
#include <MovingSprite.h>
#include <QuadTreeScene.h>
#include <SpriteDeleter.h>
#include <SoundManager.h>
#include <SpritePool.h>
#include <SpriteController.h>
#include <ResourceManager.h>

#define BULLET_SPEED sf::Vector2f(0, -160.0f)

using namespace std;

std::shared_ptr<QuadTreeScene> scene;

class Bullet : public MovingSprite
{
public:
    Bullet()
    {
        auto textureManager = Application::getInstance()->getComponent<ResourceManager<sf::Texture>>();
        auto texture = textureManager->loadFromFile("../resource/images/bullet.png");
        addTexture(*texture);
        setVelocity(BULLET_SPEED);
        setSpriteGroup(SpriteGroupID_Bullet);
    }
};

std::shared_ptr<MovingSprite> createSprite(const std::string &image, float x, float y)
{
    auto sprite = std::make_shared<MovingSprite>();
    sprite->setPosition(x, y);
    sprite->setSpriteStatus(SpriteStatus_Normal);

    auto textureManager = Application::getInstance()->getComponent<ResourceManager<sf::Texture>>();
    auto texture = textureManager->loadFromFile(image);
    sprite->addTexture(*texture, sf::IntRect());
    auto size = texture->getSize();
    sprite->setSize(size.x, size.y);
    return sprite;
}

class MyQuadTreeScene : public QuadTreeScene
{
public:
    MyQuadTreeScene()
    {
    }
public:
    void onConllision(SpritePointer current, SpritePointer target) override
    {
        auto animation = createAnimation(current->getPosition());
        addChild(animation);

        target->setSpriteStatus(SpriteStatus_Death);

        auto message = std::make_shared<SoundMessagePlaySound>("../resource/sound/blast.wav");
        Application::getInstance()->getComponent<SoundManager>()->process(message);
    }
private:
    ObjectPointer createAnimation(const sf::Vector2f &pos)
    {
        std::vector<sf::IntRect> areas;

        for (int i = 0; i < 6; i++) {
            auto area = sf::IntRect(i * 85, 0, 85, 85);
            areas.push_back(area);
        }

        std::shared_ptr<Animation> animation = std::make_shared<Animation>();
        animation->setSize(85, 85);
        animation->setTexture("../resource/images/blast2.png", areas);
        animation->setDurationPerFrame(0.12f);
        animation->setSingleShot(true);
        animation->setCenter(pos);
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

                auto spritePool = scene->getComponent<SpritePool<Bullet>>();
                std::shared_ptr<Bullet> bullet = spritePool->createOrAwakeSprite();

                auto position = sprite->getCenter();
                position.y -= sprite->getSize().y;

                bullet->setCenter(position);
                bullet->setVelocity(BULLET_SPEED);
                bullet->setSpriteGroup(SpriteGroupID_Bullet);
                scene->addChild(bullet);

                auto message = std::make_shared<SoundMessagePlaySound>("../resource/sound/shot.wav");
                Application::getInstance()->getComponent<SoundManager>()->process(message);
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

class BossSpriteController : public SpriteController
{
public:
    void setScene(SceneWeakPointer inputScene) override
    {
        scene = inputScene;
    }

    void updateSprite(SpritePointer sprite, float deltaTime) override
    {
        if (scene.expired())
            return;

        auto movingSprite = std::dynamic_pointer_cast<MovingSprite>(sprite);
        if (!movingSprite)
            return;

        movingSprite->update(deltaTime);

        auto box = movingSprite->getBoundingBox();
        if (box.top + box.width > 240) {
            movingSprite->setAcclerate(sf::Vector2f(0, -2));
        } else if (box.top < -30) {
            movingSprite->setVelocity(sf::Vector2f(0, 1));
            movingSprite->setAcclerate(sf::Vector2f(0, 1.6));
        }
    }
private:
    SceneWeakPointer scene;
};


int main()
{
    auto app = Application::getInstance();
    app->addComponent(std::make_shared<SoundManager>());

    auto message = std::make_shared<SoundMessagePlayBackgroundMusic>("../resource/sound/BGM/BGM21.WAV");
    app->getComponent<SoundManager>()->process(message);

    app->setBackgroundColor(sf::Color::Black);

    auto size = sf::Vector2f(960, 640);
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Chapter-18",
                  sf::Style::Close);
    window->setVerticalSyncEnabled(true);
    app->setWindow(window);

    scene = std::make_shared<MyQuadTreeScene>();
    scene->setName("scene");

    auto spritePool = std::make_shared<SpritePool<Bullet>>();
    scene->addComponent(spritePool);

    scene->addConllisionGroupID(SpriteGroupID_PlayerB);

    auto spriteDeleter = SpriteDeleter::create(SpriteDeleter_Slop);
    scene->addSpriteDeleter(SpriteGroupID_Bullet, spriteDeleter);

    spriteDeleter = SpriteDeleter::create(SpriteDeleter_Direct);
    scene->addSpriteDeleter(SpriteGroupID_Bullet, spriteDeleter);

    auto textureManager = Application::getInstance()->getComponent<ResourceManager<sf::Texture>>();
    auto background = textureManager->loadFromFile("../resource/images/background.png");
    scene->setBackground(*background);

    auto sprite = createSprite("../resource/images/plane.png", size.x * 0.5f, 600);
    sprite->setSpriteGroup(SpriteGroupID_PlayerA);
    scene->addMessageListener(std::make_shared<SpriteMessageListener>(sprite));
    scene->addChild(sprite);

    auto enemy = createSprite("../resource/images/enemy1.png", size.x * 0.5f, 40);
    enemy->setSpriteGroup(SpriteGroupID_PlayerB);
    enemy->setVelocity(sf::Vector2f(0, 1));
    enemy->setAcclerate(sf::Vector2f(0, 1));
    scene->addChild(enemy);

    scene->addSpriteController(SpriteGroupID_PlayerB, std::make_shared<BossSpriteController>());

    auto font = std::make_shared<sf::Font>();
    font->loadFromFile("../resource/FZYTK.TTF");

    auto text = createText(font);
    text->setText(L"????????????", false);
    text->setPosition(30, 30);
    scene->addChild(text);

    auto sceneManager = std::make_shared<SceneManager>();
    sceneManager->setInitialScene(scene);
    app->execute(sceneManager);

    return 0;
}
