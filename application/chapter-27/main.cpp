#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <Application.h>
#include <Entity.h>
#include <TileMap.h>
#include <Scene.h>
#include <Text.h>

using namespace std;

class MouseListener : public MessageListener
{
public:
    MouseListener(TileMapPointer map)
    {
        tileMap = map;
    }

    bool onListener(std::shared_ptr<Message> message) override
    {
        if(message->getType() == Message_SFML) {
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                auto mousePosition = sf::Mouse::getPosition(*Application::getInstance()->getWindow());
                auto index = tileMap.lock()->getTileIndexByWorldPosition(mousePosition.x, mousePosition.y);
                auto list = tileMap.lock()->getAdjacentTileByTileIndex(index.x, index.y);
                for(auto item : list) {
                    auto tile = tileMap.lock()->getTileByIndex(item);
                    if(tile)
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
        tileMap = TileMap::createTileMap(TileMapType_Grid);
        tileMap->init(19, 14, 36);
        tileMap->setTextVisible(true);
        tileMap->addMessageListener(std::make_shared<MouseListener>(tileMap));

        addChild(tileMap);

        setName("scene");
        auto text = createToastText();
        text->setText(L"四边形地图", false);
        addChild(text);
    }
private:
    std::shared_ptr<TileMap> tileMap;
};

int main()
{
    auto size = sf::Vector2f(960, 960);
    auto setting = sf::ContextSettings();
    setting.antialiasingLevel = 12;
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Chapter-27",
                  sf::Style::Close, setting);
    window->setVerticalSyncEnabled(true);

    auto app = Application::getInstance();
    app->setBackgroundColor(sf::Color::Blue);
    app->setWindow(window);

    //auto view = sf::View(sf::Vector2f(100, 100), sf::Vector2f(1280, 960));
    //window->setView(view);

    auto scene = std::make_shared<RectMapScene>();
    app->execute(scene);
    return 0;
}


