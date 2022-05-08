#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Entity.h>
#include <Text.h>
#include <Scene.h>
#include <Sprite.h>
#include <Animation.h>
#include <MovingSprite.h>
#include <QuadTreeScene.h>

#define BULLET_SPEED sf::Vector2f(0, -160.0f)

using namespace std;

std::shared_ptr<QuadTreeScene> scene;

std::shared_ptr<Sprite> createSprite(const std::string &image, float x, float y)
{
    auto sprite = std::make_shared<Sprite>();
    sprite->setPosition(x, y);
    sprite->setSpriteStatus(SpriteStatus_Normal);
    auto texture = Application::getInstance()->loadTexture(image);
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
    void onConllision(SpritePointer current, const std::set<SpritePointer> &sprites) override
    {
        (void)sprites;
        auto animation = createAnimation(current->getPosition());
        addChild(animation);
    }
private:
    std::shared_ptr<Animation> createAnimation(const sf::Vector2f& pos)
    {
        std::vector<sf::IntRect> areas;

        for(int i = 0; i < 6; i++) {
            auto area = sf::IntRect(i * 85, 0, 85, 85);
            areas.push_back(area);
        }

        std::shared_ptr<Animation> animation = std::make_shared<Animation>();
        animation->setDurationPerFrame(0.2f);
        animation->setSingleShot(true);
        animation->setPosition(pos);

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
                auto texture = Application::getInstance()->loadTexture("../resource/images/bullet.png");
                auto bullet = std::make_shared<MovingSprite>();
                bullet->addTexture(*texture);

                auto position = sprite->getPosition();
                position.y -= sprite->getSize().y;

                bullet->setPosition(position);
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
    auto size = sf::Vector2f(960, 640);
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Chapter-8",
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

    auto background = Application::getInstance()->loadTexture("../resource/images/background.png");
    scene->setBackground(*background);

    auto sprite = createSprite("../resource/images/plane.png", size.x * 0.5f, 600);
    sprite->setSpriteGroup(SpriteGroupID_PlayerB);
    scene->addMessageListener(std::make_shared<SpriteMessageListener>(sprite));
    scene->addChild(sprite);

    auto enemy = createSprite("../resource/images/enemy1.png", size.x * 0.5f, 40);
    enemy->setSpriteGroup(SpriteGroupID_PlayerB);
    scene->addChild(enemy);

    auto font = std::make_shared<sf::Font>();
    font->loadFromFile("../resource/FZYTK.TTF");

    auto text = createText(font);
    text->setText(L"四叉树场景", false);
    text->setPosition(80, 30);
    scene->addChild(text);

    auto sceneManager = std::make_shared<SceneManager>();
    sceneManager->setInitialScene(scene);
    app->execute(sceneManager);

    return 0;
}
