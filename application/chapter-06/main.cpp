#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Scene.h>
#include <Animation.h>
#include <Text.h>

#define WIN_SIZE 640

using namespace std;

ObjectPointer createAnimation()
{
    std::vector<sf::IntRect> areas;

    for(int i = 0; i < 6; i++) {
        auto area = sf::IntRect(i * 85, 0, 85, 85);
        areas.push_back(area);
    }

    std::shared_ptr<Animation> animation = std::make_shared<Animation>();
    animation->setDurationPerFrame((rand() % 20) * 0.01f + 0.2f);
    animation->setSingleShot(false);
    animation->setPosition(
        rand() % WIN_SIZE, rand() % WIN_SIZE);

    animation->setBackgroundColor(sf::Color::White);
    animation->setTexture("../resource/images/blast2.png", areas);
    animation->start();
    return animation;
}

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
    auto size = sf::Vector2f(WIN_SIZE, WIN_SIZE);
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Chapter-6",
                  sf::Style::Close);
    window->setVerticalSyncEnabled(true);

    auto app = Application::getInstance();
    app->setBackgroundColor(sf::Color::Black);
    app->setWindow(window);

    auto scene = std::make_shared<Scene>();
    scene->setName("scene");

    auto background = Application::getInstance()->loadTexture("../resource/images/grid.png");
    scene->setBackground(*background);

    for(int i = 0; i < 100; i++)
        scene->addChild(createAnimation());

    auto font = std::make_shared<sf::Font>();
    font->loadFromFile("../resource/FZYTK.TTF");

    auto text = createText(font);
    text->setText(L"动画", false);
    text->setPosition(80, 30);
    scene->addChild(text);

    auto sceneManager = std::make_shared<SceneManager>();
    sceneManager->setInitialScene(scene);
    app->execute(sceneManager);

    return 0;
}

