#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Text.h>
#include <Scene.h>
#include <TileMap.h>
#include <SoundManager.h>
#include <Camera.h>
#include <Util.h>
#include <ResourceManager.h>
#include <HelpListener.h>
#include <Widget/Desktop.h>
#include <Widget/TableLayout.h>
#include <Widget/ImageBox.h>
#include <Widget/Label.h>
#include <Widget/Panel.h>
#include <CameraTrackingObject.h>
#include <iostream>

using namespace std;

#define APP_SIZE sf::Vector2f(960,720)

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
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                auto camera = Application::getInstance()->getComponent<Camera>();
                auto view = camera->getView();
                auto center = view.getCenter();
                center.x -= 5.0f;
                camera->setCenter(center);
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                auto camera = Application::getInstance()->getComponent<Camera>();
                auto view = camera->getView();
                auto center = view.getCenter();
                center.x += 5.0f;
                camera->setCenter(center);
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                auto camera = Application::getInstance()->getComponent<Camera>();
                auto view = camera->getView();
                auto center = view.getCenter();
                center.y -= 5.0f;
                camera->setCenter(center);
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                auto camera = Application::getInstance()->getComponent<Camera>();
                auto view = camera->getView();
                auto center = view.getCenter();
                center.y += 5.0f;
                camera->setCenter(center);
            } else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                auto mousePosition = sf::Mouse::getPosition(*Application::getInstance()->getWindow());
                sf::Vector2f position = Application::getInstance()->getWindow()->mapPixelToCoords(mousePosition);
                auto index = tileMap.lock()->getTileIndexByWorldPosition(position.x, position.y);
                auto tile = tileMap.lock()->getTileByIndex(index);
                if (tile) {
                    tile->setFillColor(sf::Color::Blue);
                }
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
        tileMap = TileMap::createTileMap(TileMapType_Hex);
        tileMap->init(56, 44, 48);

        addChild(tileMap);

        setName("scene");

        auto text = createToastText();
        text->setText(L"摄像机跟随", false);

        auto cameraTracking = std::make_shared<CameraTrackingObject>();
        cameraTracking->setTarget(text);
        cameraTracking->setTargetPosition(sf::Vector2f(20, 20));

        addChild(cameraTracking);

        tileMap->setTextVisible(true);
        tileMap->accept(this);

        Application::getInstance()->getComponent<Camera>()->setGlobalArea(tileMapArea);

        auto mouseListener = std::make_shared<MouseListener>(tileMap);
        addMessageListener(mouseListener);
    }

    void visit(uint32_t x, uint32_t y, std::shared_ptr<Tile> tile) override
    {
        (void)x, (void)y;
        tile->setScale(0.95f, 0.95f);
        tile->setVisible(true);
        tile->setFillColor(sf::Color(rand() % 250, rand() % 250, rand() % 250));

        auto box = tile->getGlobalBounds();
        tileMapArea = expandRect(tileMapArea, box);
    }
private:
    std::shared_ptr<TileMap> tileMap;
    sf::FloatRect tileMapArea;
};

int main()
{
    auto size = sf::Vector2f(APP_SIZE);
    auto setting = sf::ContextSettings();
    setting.antialiasingLevel = 12;
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Chapter-34",
                  sf::Style::Close, setting);
    window->setVerticalSyncEnabled(true);

    Application::getInstance()->setWindow(window);
    auto camera = std::make_shared<Camera>();
    Application::getInstance()->addComponent(camera);
    camera->setWindowArea(sf::FloatRect(0, 0, APP_SIZE.x, APP_SIZE.y));

    auto scene = std::make_shared<RectMapScene>();
    auto sceneManager = std::make_shared<SceneManager>();
    sceneManager->addScene(scene);
    sceneManager->setInitialScene(scene);

    Application::getInstance()->execute(sceneManager);
    return 0;
}
