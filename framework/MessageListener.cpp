#include <Message.h>

bool MousePressedListener::onListener(std::shared_ptr<Message> message)
{
    if (message->getType() == Message_SFML) {
        auto sfml = std::dynamic_pointer_cast<SFMLMessage>(message);
        auto event = sfml->getEvent();
        return event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == button;
    }
    return false;
}

bool MouseReleasedListener::onListener(std::shared_ptr<Message> message)
{
    if (message->getType() == Message_SFML) {
        auto sfml = std::dynamic_pointer_cast<SFMLMessage>(message);
        auto event = sfml->getEvent();
        return event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == button;
    }
    return false;
}

bool MouseWheelListener::onListener(std::shared_ptr<Message> message)
{
    if (message->getType() == Message_SFML) {
        auto sfml = std::dynamic_pointer_cast<SFMLMessage>(message);
        auto event = sfml->getEvent();
        if (event.type == sf::Event::MouseWheelScrolled)
            return true;
    }
    return false;
}

void MouseDoubleClickListener::setClickMaxInterval(float clickInterval)
{
    clickMaxInterval = clickInterval;
}

bool MouseDoubleClickListener::onListener(std::shared_ptr<Message> message)
{
    if (message->getType() == Message_SFML) {
        auto sfml = std::dynamic_pointer_cast<SFMLMessage>(message);
        auto event = sfml->getEvent();
        if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left && clock.restart().asSeconds() < clickMaxInterval)
                return true;
        }

        return false;
    }
    return false;
}

KeyPressedListener::KeyPressedListener(sf::Keyboard::Key inputCode, bool inputAlt, bool inputCtrl,
                                       bool inputShift):
    code(inputCode),
    alt(inputAlt),
    ctrl(inputAlt),
    shift(inputShift)
{
}

bool KeyPressedListener::onListener(std::shared_ptr<Message> message)
{
    if (message->getType() == Message_SFML) {
        auto sfml = std::dynamic_pointer_cast<SFMLMessage>(message);
        auto event = sfml->getEvent();
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == code && event.key.alt == alt && event.key.control == ctrl
                    && event.key.shift == shift)
                return true;
        }

        return false;
    }
    return false;
}
