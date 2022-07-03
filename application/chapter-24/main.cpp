#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Text.h>
#include <Scene.h>
#include <ResourceManager.h>
#include <SoundManager.h>
#include <Widget/Desktop.h>
#include <Widget/BoxLayout.h>
#include <Widget/Button.h>

using namespace std;

class ExitListener : public MessageListener
{
    // MessageListener interface
public:
    bool onListener(std::shared_ptr<Message> message)
    {
        if(message->getType() == Message_EXIT) {
            Application::getInstance()->exit();
            return true;
        }
        return false;
    }
};

int main()
{
    auto size = sf::Vector2f(1152, 720);
    auto setting = sf::ContextSettings();
    setting.antialiasingLevel = 12;
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Chapter-24",
                  sf::Style::Close, setting);
    window->setVerticalSyncEnabled(true);

    auto app = Application::getInstance();
    app->addMessageListener(std::make_shared<ExitListener>());
    app->addComponent(std::make_shared<SoundManager>());

    auto message = std::make_shared<SoundMessagePlayBackgroundMusic>("../resource/sound/BGM/BGM02.WAV");
    app->getComponent<SoundManager>()->process(message);
    app->setBackgroundColor(sf::Color::Blue);
    app->setWindow(window);

    auto scene = std::make_shared<Scene>();
    auto imageManager = Application::getInstance()->getComponent<ResourceManager<sf::Image>>();
    auto image = imageManager->loadFromFile("../resource/images/tank/background.png");

    sf::Texture texture;
    texture.loadFromImage(*image);
    scene->setBackground(texture);
    auto desktop = std::make_shared<Desktop>();
    scene->addComponent(desktop);

    auto boxLayout = std::make_shared<VBoxLayout>();
    boxLayout->setSpacing(3.0f);
    boxLayout->setMargin(3.0f);
    desktop->addWidget(boxLayout, HMode_Center, VMode_Center);

    auto buttonStyle = std::make_shared<ButtonStyle>();
    buttonStyle->size = 18.0f;
    buttonStyle->outlineStyle = {sf::Color::Blue, 3.0f};

    {
        auto button = std::make_shared<Button>();
        button->setWidgetStyle(buttonStyle);
        button->setSize(200, 60);
        button->setText(L"载入");
        boxLayout->addWidget(button, HMode_Center);
    }
    {
        auto button = std::make_shared<Button>();
        button->setWidgetStyle(buttonStyle);
        button->setSize(200, 60);
        button->setText(L"新游戏");
        boxLayout->addWidget(button, HMode_Center);
    }
    {
        auto button = std::make_shared<Button>();
        button->setWidgetStyle(buttonStyle);
        button->setSize(200, 60);
        button->setText(L"退出");
        button->released.connect(std::bind(&Application::exit, Application::getInstance()));
        boxLayout->addWidget(button, HMode_Center);
    }

    boxLayout->setBackgroundColor(sf::Color::Black);

    app->execute(scene);
    return 0;
}

