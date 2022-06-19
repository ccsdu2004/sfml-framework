#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Text.h>
#include <Scene.h>
#include <Widget/Desktop.h>
#include <Widget/Label.h>

using namespace std;

class HelpListener : public MessageListener
{
    // MessageListener interface
public:
    HelpListener()
    {
        helpLabel = std::make_shared<Label>();
        helpLabel->setSize(320, 108);
        helpLabel->setText(L"帮助\n点击键盘方向键控制坦克旋转\n点击上键控制坦克前进\n点击空格弹出本提示框");

        auto style = std::make_shared<LabelStyle>();
        style->textColor = sf::Color::Red;
        style->size = 21.0f;
        style->vMode = VMode_Top;
        helpLabel->setWidgetStyle(style);

        auto desktop = Application::getInstance()->getComponent<Desktop>();
        desktop->addWidget(helpLabel, HMode_Right, VMode_Top);
    }

    bool onListener(std::shared_ptr<Message> message) override
    {
        if(message->getType() == Message_SFML) {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {
                helpLabel->setVisible(!helpLabel->isVisible());
                return true;
            }
        }
        return false;
    }
private:
    std::shared_ptr<Label> helpLabel;
};

int main()
{
    auto size = sf::Vector2f(800, 600);
    auto setting = sf::ContextSettings();
    setting.antialiasingLevel = 12;
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Chapter-23",
                  sf::Style::Close, setting);
    window->setVerticalSyncEnabled(true);

    auto app = Application::getInstance();
    app->setBackgroundColor(sf::Color::Blue);
    app->setWindow(window);

    auto scene = std::make_shared<Scene>();
    auto helpListener = std::make_shared<HelpListener>();
    scene->addMessageListener(helpListener);

    auto text = scene->createToastText();
    text->setText(L"按F1弹出帮助提示", false);
    text->setPosition(30, 30);
    scene->addChild(text);

    app->execute(scene);
    return 0;
}

