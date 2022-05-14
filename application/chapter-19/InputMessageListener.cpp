#include "InputMessageListener.h"
#include "Application.h"
#include "StateMachine.h"
#include "MineSweeperScene.h"

InputMessageListener::InputMessageListener(MineSweeperScene& inputScene):
    scene(inputScene)
{
}

bool InputMessageListener::onListener(std::shared_ptr<Message> message)
{
    auto sfml = std::dynamic_pointer_cast<SFMLMessage>(message);
    auto event = sfml->getEvent();
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Key::Left) {
            scene.makeSpriteTurnLeft();
            return true;
        } else if (event.key.code == sf::Keyboard::Key::Right) {
            scene.makeSpriteTurnRight();
            return true;
        } else if (event.key.code == sf::Keyboard::Key::Up) {
            auto switchMessage = std::make_shared<StateSwitchMessage>("forward");
            scene.makeSpriteForward();
            return true;
        }
    }

    return false;
}
