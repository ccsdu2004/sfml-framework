#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Entity.h>
#include <TileMap.h>
#include <Scene.h>
#include <Text.h>

using namespace std;

std::shared_ptr<SceneManager> sceneManager;

class SceneListener : public MessageListener
{
public:
    bool onListener(std::shared_ptr<Message> message) override
    {
        if (message->getType() == Message_SFML) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                auto scene = sceneManager->getCurrentScene()->getName();
                if (scene == "scene1")
                    sceneManager->switchTo("scene2");
                else
                    sceneManager->switchTo("scene1");
                return true;
            }
        }
        return false;
    }
};

class MouseListener : public MessageListener
{
public:
    MouseListener(TileMapPointer map)
    {
        tileMap = map;
    }

    bool onListener(std::shared_ptr<Message> message) override
    {
        if (message->getType() == Message_SFML) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                auto mousePosition = sf::Mouse::getPosition(*Application::getInstance()->getWindow());
                auto index = tileMap.lock()->getTileIndexByWorldPosition(mousePosition.x, mousePosition.y);
                auto list = tileMap.lock()->getAdjacentTileByTileIndex(index.x, index.y);
                for (auto item : list) {
                    auto tile = tileMap.lock()->getTileByIndex(item);
                    if (tile)
                        tile->setFillColor(sf::Color(rand() % 250, rand() % 250, rand() % 250, 128));
                }
                return true;
            }
        }
        return false;
    }
private:
    std::weak_ptr<TileMap> tileMap;
};

class RectMapScene : public Scene
{
public:
    RectMapScene()
    {
        tileMap = TileMap::createTileMap(TileMapType_MalGrid);
        tileMap->init(21, 19, 64);
        tileMap->setTextVisible(true);
        tileMap->addMessageListener(std::make_shared<MouseListener>(tileMap));

        addChild(tileMap);

        setName("scene1");
        auto text = createToastText();
        text->setText(L"四边形地图1", false);
        addChild(text);
    }
private:
    std::shared_ptr<TileMap> tileMap;
};

class Scene2 : public Scene
{
public:
    Scene2()
    {
        tileMap = TileMap::createTileMap(TileMapType_Grid);
        tileMap->init(21, 19, 64);
        tileMap->setTextVisible(true);
        tileMap->addMessageListener(std::make_shared<MouseListener>(tileMap));

        addChild(tileMap);

        setName("scene2");
        auto text = createToastText();
        text->setText(L"四边形地图2", false);
        addChild(text);
    }
private:
    TileMapPointer tileMap;
};

int main()
{
    auto size = sf::Vector2f(1280, 960);
    auto setting = sf::ContextSettings();
    setting.antialiasingLevel = 12;
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Chapter-26",
                                                     sf::Style::Close, setting);
    window->setVerticalSyncEnabled(true);

    auto app = Application::getInstance();
    app->setBackgroundColor(sf::Color::Blue);
    app->setWindow(window);

    sceneManager = std::make_shared<SceneManager>();
    sceneManager->addMessageListener(std::make_shared<SceneListener>());
    auto scene1 = std::make_shared<RectMapScene>();
    auto scene2 = std::make_shared<Scene2>();
    sceneManager->setInitialScene(scene1);
    sceneManager->addScene(scene2);

    app->execute(sceneManager);
    return 0;
}


