#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Text.h>
#include <Scene.h>
#include <SoundManager.h>
#include <ResourceManager.h>
#include <Widget/Desktop.h>
#include <Widget/TableLayout.h>
#include <Widget/ImageBox.h>
#include <Widget/Panel.h>

using namespace std;

class ImageBoxPanel : public Panel
{
public:
    void create()
    {
        auto style = std::make_shared<PanelStyle>();
        style->titleStyle->normalColor = sf::Color(60, 60, 60, 255);
        setWidgetStyle(style);

        setTitle(L"瓦片编辑器");

        auto tableLayout = std::make_shared<TableLayout>(3, 3);
        tableLayout->setSpacing(2.0f);

        auto imageManager = Application::getInstance()->getComponent<ResourceManager<sf::Image>>();

        auto tileSet = imageManager->loadFromFile("../resource/images/tileset.png");

        textures.resize(9);

        std::shared_ptr<ImageBoxStyle> imageBoxStyle = std::make_shared<ImageBoxStyle>();
        //imageBoxStyle->outlinePressedStyle = {sf::Color::Red, 1.2f};
        //imageBoxStyle->outlineStyle = {sf::Color::Blue, 1.2f};

        for(uint32_t i = 0; i < 9; i++) {
            auto imageBox = std::make_shared<ImageBox>(sf::Vector2f(32, 32));
            imageBox->setWidgetStyle(imageBoxStyle);

            sf::IntRect rect;
            rect.left = 0 + (i % 3) * 32;
            rect.top = 0 + (i / 3) * 32;
            rect.width = 32;
            rect.height = 32;

            textures[i].loadFromImage(*tileSet, rect);
            imageBox->setTexture(textures[i]);
            tableLayout->addWidget(imageBox, i / 3, i % 3);
        }
        setContextWidget(tableLayout);
    }
private:
    std::vector<sf::Texture> textures;
};

int main()
{
    auto size = sf::Vector2f(960, 720);
    auto setting = sf::ContextSettings();
    setting.antialiasingLevel = 12;
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Chapter-31",
                  sf::Style::Close, setting);
    window->setVerticalSyncEnabled(true);

    auto app = Application::getInstance();
    app->setWindow(window);

    auto scene = std::make_shared<Scene>();
    auto sceneManager = std::make_shared<SceneManager>();
    sceneManager->addScene(scene);
    sceneManager->setInitialScene(scene);

    auto desktop = std::make_shared<Desktop>();
    scene->addComponent(desktop);

    auto panel = std::make_shared<ImageBoxPanel>();
    panel->create();

    desktop->addWidget(panel, HMode_Center, VMode_Center);
    app->execute(sceneManager);
    return 0;
}
