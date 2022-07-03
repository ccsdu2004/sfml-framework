#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <Application.h>
#include <Entity.h>
#include <TileMap.h>
#include <Scene.h>
#include <Text.h>
#include <ResourceManager.h>

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
        if (message->getType() == Message_SFML) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                auto mousePosition = sf::Mouse::getPosition(*Application::getInstance()->getWindow());
                auto index = tileMap.lock()->getTileIndexByWorldPosition(mousePosition.x, mousePosition.y);
                auto tile = tileMap.lock()->getTileByIndex(index);
                if (tile)
                    tile->setOutlineColor(sf::Color::Yellow);
            }
            return true;
        }

        return false;
    }
private:
    std::weak_ptr<TileMap> tileMap;
};

class RectMapScene : public Scene, public TileVisitor
{
public:
    RectMapScene()
    {
        tileMap = TileMap::createTileMap(TileMapType_Grid);
        tileMap->init(19, 19, 64);

        addChild(tileMap);

        setName("scene");
        auto text = createToastText();
        text->setText(L"瓦片地图", false);
        addChild(text);

        auto imageManager = Application::getInstance()->getComponent<ResourceManager<sf::Image>>();

        tileSet = *imageManager->loadFromFile("../resource/images/tileset.png");

        for(int i = 0; i < 8; i++)
            for(int j = 0; j < 6; j++) {
                sf::IntRect rect;
                rect.left = i * 32;
                rect.top = j * 32;
                rect.width = 32;
                rect.height = 32;

                std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();
                texture->loadFromImage(tileSet, rect);
                textures.push_back(texture);
            }

        tileMap->accept(this);

        auto listener = std::make_shared<MouseListener>(tileMap);
        tileMap->addMessageListener(listener);
    }

    void visit(uint32_t x, uint32_t y, std::shared_ptr<Tile> tile) override
    {
        (void)x, (void)y;
        tile->setScale(0.9f, 0.9f);
        tile->setVisible(true);
        tile->setFillColor(sf::Color::White);
        int id = rand() % textures.size();
        tile->setTexture(textures.at(id).get());
    }
private:
    std::shared_ptr<TileMap> tileMap;
    sf::Image tileSet;
    std::vector<std::shared_ptr<sf::Texture>> textures;
};

int main()
{
    auto size = sf::Vector2f(960, 720);
    auto setting = sf::ContextSettings();
    setting.antialiasingLevel = 12;
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Chapter-30",
                  sf::Style::Close, setting);
    window->setVerticalSyncEnabled(true);

    auto app = Application::getInstance();
    app->setWindow(window);

    auto scene = std::make_shared<RectMapScene>();
    app->execute(scene);
    return 0;
}

