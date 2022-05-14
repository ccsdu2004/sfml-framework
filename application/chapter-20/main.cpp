#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Entity.h>
#include <TileMap.h>
#include <Text.h>
#include <Scene.h>
#include <ShortestPathFinder.h>
#include <Widget/Desktop.h>
#include <Widget/Widget.h>
#include <Widget/MouseArea.h>

using namespace std;

#define START_TILE (sf::Vector2i(0,0))
#define END_TILE (sf::Vector2i(22,14))

#define BLOCK_COLOR sf::Color::Red
#define PATH_COLOR sf::Color::Blue
#define NORMAL_COLOR sf::Color::Black

static const int ID_OFFSET = 100;

inline size_t getIDByPosition(int x, int y)
{
    return x * ID_OFFSET + y;
}

inline sf::Vector2i getPositionByID(size_t id)
{
    return sf::Vector2i(id / ID_OFFSET, id % ID_OFFSET);
}

class TileMessageListener : public MessageListener, public TileVisitor
{
    // MessageListener interface
public:
    TileMessageListener() = delete;
    TileMessageListener(TileMapPointer inputTileMap):
        tileMap(inputTileMap)
    {
        shortestPathFinder = std::make_shared<ShortestPathFinder>();
    }
public:
    bool onListener(std::shared_ptr<Message> message) override
    {
        auto sfml = std::dynamic_pointer_cast<SFMLMessage>(message);
        auto event = sfml->getEvent();
        if (event.type == sf::Event::MouseButtonPressed) {
            auto mousePosition = sf::Mouse::getPosition(*Application::getInstance()->getWindow());
            auto index = tileMap->getTileIndexByWorldPosition(mousePosition.x, mousePosition.y);

            if (index != START_TILE && index != END_TILE) {
                auto tile = tileMap->getTileByIndex(index.x, index.y);
                if (tile->getFillColor() == BLOCK_COLOR)
                    tile->setFillColor(NORMAL_COLOR);
                else
                    tile->setFillColor(BLOCK_COLOR);
            }
            return true;
        } else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Key::Space) {
                compute();
                return true;
            }
        }

        return false;
    }

    void compute()
    {
        shortestPathFinder->clear();

        tileMap->accept(this);

        std::vector<uint32_t> output;
        shortestPathFinder->search(getIDByPosition(START_TILE.x, START_TILE.y), getIDByPosition(END_TILE.x,
                                                                                                END_TILE.y), output);

        auto itr = output.begin();
        while (itr != output.end()) {
            auto position = getPositionByID(*itr);
            if (position != START_TILE && position != END_TILE)
                tileMap->getTileByIndex(position.x, position.y)->setFillColor(PATH_COLOR);
            itr ++;
        }
    }

    void visit(uint32_t x, uint32_t y, std::shared_ptr<Tile> tile) override
    {
        if (tile->getFillColor() == PATH_COLOR)
            tile->setFillColor(NORMAL_COLOR);

        auto adjList = tileMap->getAdjacentTileByPosition(x, y);
        auto itr = adjList.begin();
        while (itr != adjList.end()) {
            auto adjTile = tileMap->getTileByIndex(itr->x, itr->y);
            if (adjTile && adjTile->getFillColor() != BLOCK_COLOR) {
                shortestPathFinder->addEdge(getIDByPosition(x, y), getIDByPosition(itr->x, itr->y), 1);
            }
            itr ++;
        }
    }
private:
    TileMapPointer tileMap;
    std::shared_ptr<ShortestPathFinder> shortestPathFinder;
};

int main()
{
    auto size = sf::Vector2f(800, 600);
    auto setting = sf::ContextSettings();
    setting.antialiasingLevel = 12;
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Chapter-20",
                                                     sf::Style::Close, setting);
    window->setVerticalSyncEnabled(true);

    auto app = Application::getInstance();
    app->setBackgroundColor(sf::Color::Blue);
    app->setWindow(window);

    auto tileMap = TileMap::createTileMap(TileMapType_Hex);
    tileMap->init(23, 15, 24);
    tileMap->setMessageReceived(true);
    tileMap->setTextVisible(true);

    auto listener = std::make_shared<TileMessageListener>(tileMap);
    tileMap->addMessageListener(listener);

    tileMap->getTileByIndex(START_TILE.x, START_TILE.y)->setFillColor(sf::Color::Magenta);
    tileMap->getTileByIndex(END_TILE.x, END_TILE.y)->setFillColor(sf::Color::Magenta);

    auto scene = std::make_shared<Scene>();
    auto desktop = std::make_shared<Desktop>();
    scene->addComponent(desktop);
    scene->addChild(tileMap);

    auto panel = std::make_shared<Widget>(sf::Vector2f(100.0f, 100.0f));
    panel->setPosition(100, 100);
    desktop->addWidget(panel, HMode_Center, VMode_Center);

    auto mouseArea = std::make_shared<MouseArea>(sf::Vector2f(100.0f, 100.0f));
    desktop->addWidget(mouseArea, 0, 0);
    mouseArea->setBackgroundColor(sf::Color::Blue);
    mouseArea->setTargetWidget(panel);


    auto text = scene->createToastText();
    text->setText(L"最短路径", false);
    text->setPosition(80, 30);
    scene->addChild(text);

    app->execute(scene);
    return 0;
}

