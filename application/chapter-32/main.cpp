#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Text.h>
#include <Scene.h>
#include <TileMap.h>
#include <SoundManager.h>
#include <ResourceManager.h>
#include <Widget/Desktop.h>
#include <Widget/TableLayout.h>
#include <Widget/ImageBox.h>
#include <Widget/Panel.h>

using namespace std;

class TileImageSelector : public Panel
{
public:
    void create()
    {
        auto style = std::make_shared<PanelStyle>();
        style->titleStyle->normalColor = sf::Color(60, 60, 60, 255);
        style->outlineStyle = {sf::Color(98, 98, 98, 250), 3.2f};
        style->normalColor = sf::Color(32, 32, 32, 240);
        setWidgetStyle(style);

        setTitle(L"瓦片编辑器");

        auto tableLayout = std::make_shared<TableLayout>(12, 4);
        tableLayout->setSpacing(2.0f);

        auto textureManager = Application::getInstance()->getComponent<ResourceManager<sf::Texture>>();

        std::shared_ptr<ImageBoxStyle> imageBoxStyle = std::make_shared<ImageBoxStyle>();
        imageBoxStyle->pressedColor = sf::Color(228, 128, 128, 240);
        imageBoxStyle->outlinePressedStyle = {sf::Color::Blue, 2.4f};

        uint32_t row = 0;
        uint32_t col = 0;

        std::shared_ptr<WidgetGroup> widgetGroup = std::make_shared<WidgetGroup>();

        for(int i = 0; i < 8; i++)
            for(int j = 0; j < 6; j++) {
                sf::IntRect rect;
                rect.left = i * 32;
                rect.top = j * 32;
                rect.width = 32;
                rect.height = 32;

                auto name = std::to_string(i) + "," + std::to_string(j);
                auto texture = textureManager->loadFromFile("../resource/images/tileset.png", name, rect);

                auto imageBox = std::make_shared<ImageBox>(sf::Vector2f(32, 32));
                imageBox->setWidgetGroup(widgetGroup);
                imageBox->clicked.connect([&](std::shared_ptr<ImageBox> imageBox) {
                    onImageBoxClicked(imageBox);
                });
                imageBox->setWidgetStyle(imageBoxStyle);
                imageBox->setTexture(*texture);
                tableLayout->addWidget(imageBox, row, col);
                col ++;
                if(col % 4 == 0) {
                    col = 0;
                    row ++;
                }
            }

        setContextWidget(tableLayout);
    }
public:
    void onImageBoxClicked(std::shared_ptr<ImageBox> imageBox)
    {
        selectedImageBox = imageBox;
    }

    const sf::Texture* getTexture()const
    {
        if(!selectedImageBox)
            return nullptr;
        return selectedImageBox->getTexture();
    }
private:
    std::shared_ptr<ImageBox> selectedImageBox;
};

shared_ptr<TileImageSelector> panel;

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
                if (tile) {
                    auto texture = panel->getTexture();
                    if(texture) {
                        tile->setFillColor(sf::Color::White);
                        tile->setTexture(texture);
                    }
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
        tileMap->init(28, 22, 48);

        addChild(tileMap);

        setName("scene");
        auto text = createToastText();
        text->setText(L"地图编辑", false);
        addChild(text);

        tileMap->accept(this);

        auto listener = std::make_shared<MouseListener>(tileMap);
        tileMap->addMessageListener(listener);
    }

    void visit(uint32_t x, uint32_t y, std::shared_ptr<Tile> tile) override
    {
        (void)x, (void)y;
        tile->setScale(0.95f, 0.95f);
        tile->setVisible(true);
        tile->setFillColor(sf::Color::Green);
    }
private:
    std::shared_ptr<TileMap> tileMap;
};

int main()
{
    auto size = sf::Vector2f(960, 720);
    auto setting = sf::ContextSettings();
    setting.antialiasingLevel = 12;
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Chapter-32",
                  sf::Style::Close, setting);
    window->setVerticalSyncEnabled(true);

    Application::getInstance()->setWindow(window);

    auto scene = std::make_shared<RectMapScene>();
    auto sceneManager = std::make_shared<SceneManager>();
    sceneManager->addScene(scene);
    sceneManager->setInitialScene(scene);

    auto desktop = std::make_shared<Desktop>();
    scene->addComponent(desktop);

    panel = std::make_shared<TileImageSelector>();
    panel->create();

    desktop->addWidget(panel, HMode_Center, VMode_Center);
    Application::getInstance()->execute(sceneManager);
    return 0;
}
