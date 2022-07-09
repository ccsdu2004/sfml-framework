#include <HelpListener.h>
#include <Application.h>
#include <Widget/Desktop.h>
#include <Widget/Label.h>

class HelpListenerData
{
public:
    std::shared_ptr<Label> label;
};

HelpListener::HelpListener(const sf::String& text, const sf::Vector2f& size):
    data(new HelpListenerData)
{
    data->label = std::make_shared<Label>();
    data->label->setSize(size.x, size.y);
    data->label->setText(text);

    auto style = std::make_shared<LabelStyle>();
    style->textColor = sf::Color::Red;
    style->normalColor = sf::Color(100, 100, 180, 230);
    style->size = 21.0f;
    style->vMode = VMode_Top;
    data->label->setWidgetStyle(style);

    auto desktop = Application::getInstance()->getComponent<Desktop>();
    desktop->addWidget(data->label, HMode_Right, VMode_Top);
}

HelpListener::~HelpListener()
{
}

bool HelpListener::onListener(std::shared_ptr<Message> message)
{
    if(message->getType() == Message_SFML) {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {
            data->label->setVisible(!data->label->isVisible());
            return true;
        }
    }
    return false;
}
