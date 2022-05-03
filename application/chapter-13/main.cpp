#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Scene.h>
#include <Sprite.h>
#include <Animation.h>
#include <MovingSprite.h>
#include <QuadTreeScene.h>
#include <SpriteController.h>

#define OFFSET 10

using namespace std;

std::shared_ptr<QuadTreeScene> scene;

class MySpriteController : public SpriteController
{
public:
    void setScene(SceneWeakPointer inputScene) override
    {
        scene = inputScene;
    }

    void updateSprite(SpritePointer sprite, float deltaTime) override
    {
        sprite->move(-240.0f * deltaTime);

        if(scene.expired())
            return;

        sf::FloatRect sceneBox = scene.lock()->getBoundingBox();

        auto spritePosition = sprite->getPosition();
        if(spritePosition.y < -OFFSET)
            sprite->setPosition(spritePosition.x, sceneBox.height + OFFSET);
        else if(spritePosition.y > sceneBox.height + OFFSET)
            sprite->setPosition(spritePosition.x, -OFFSET);

        if(spritePosition.x < -OFFSET)
            sprite->setPosition(sceneBox.width + OFFSET, spritePosition.y);
        else if(spritePosition.x > sceneBox.width + OFFSET)
            sprite->setPosition(-OFFSET, spritePosition.y);
    }
private:
    SceneWeakPointer scene;
};

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

int main()
{
    auto size = sf::Vector2f(960, 640);
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Chapter-13",
                  sf::Style::Close);
    window->setVerticalSyncEnabled(true);

    auto app = Application::getInstance();
    app->setBackgroundColor(sf::Color::Black);
    app->setWindow(window);

    scene = std::make_shared<QuadTreeScene>();
    scene->setName("scene");

    auto background = Application::getInstance()->loadTexture("../resource/images/background.png");
    scene->setBackground(*background);

    auto controller = std::make_shared<MySpriteController>();
    scene->addSpriteController(SpriteGroupID_PlayerA, controller);

    for(int i = 0; i < 20; i++) {
        auto sprite = createSprite("../resource/images/plane.png", rand() % 960, rand() % 640); // size.x * 0.5f, 600);
        sprite->setRotate(-180.0f);
        sprite->setSpriteGroup(SpriteGroupID_PlayerA);
        sprite->setSpriteColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
        scene->addChild(sprite);
    }

    auto sceneManager = std::make_shared<SceneManager>();
    sceneManager->setInitialScene(scene);
    app->execute(sceneManager);

    return 0;
}