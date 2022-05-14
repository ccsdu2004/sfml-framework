#pragma once
#include <memory>
#include <list>
#include <functional>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

enum MessageType {
    Message_SFML = 0,
    Message_SOUND,
    Message_USER,
    Message_STATE,
    Message_Max
};

class Message
{
public:
    Message(MessageType inputType = Message_USER):
        type(inputType)
    {
    }

    virtual ~Message()
    {
    }

    MessageType getType()const
    {
        return type;
    }
private:
    MessageType type;
};

class SFMLMessage : public Message
{
public:
    SFMLMessage(const sf::Event &inputEvent):
        Message(Message_SFML),
        event(inputEvent)
    {
    }
public:
    sf::Event getEvent()const
    {
        return event;
    }
private:
    sf::Event event;
};

class MessageListener
{
public:
    virtual bool onListener(std::shared_ptr<Message> message) = 0;
};

class MousePressedListener : public MessageListener
{
public:
    MousePressedListener(sf::Mouse::Button inputButton):
        button(inputButton)
    {
    }
public:
    bool onListener(std::shared_ptr<Message> message);
private:
    sf::Mouse::Button button;
};

class MouseReleasedListener : public MessageListener
{
public:
    MouseReleasedListener(sf::Mouse::Button inputButton):
        button(inputButton) {}
public:
    bool onListener(std::shared_ptr<Message> message);
private:
    sf::Mouse::Button button;
};

class MouseWheelListener : public MessageListener
{
public:
    MouseWheelListener(int inputLow, int inputHigh):
        low(inputLow),
        high(inputHigh)
    {
    }
public:
    bool onListener(std::shared_ptr<Message> message);
private:
    int low, high;
};

class MouseDoubleClickListener : public MessageListener
{
public:
    MouseDoubleClickListener(float clickInterval = 0.5f):
        clickMaxInterval(clickInterval)
    {
    }
public:
    void setClickMaxInterval(float clickInterval);
    bool onListener(std::shared_ptr<Message> message);
private:
    float clickMaxInterval = 0.5;
    sf::Clock clock;
};

class KeyPressedListener : public MessageListener
{
public:
    KeyPressedListener(sf::Keyboard::Key code, bool alt = false, bool ctrl = false, bool shift = false);
public:
    bool onListener(std::shared_ptr<Message> message) override;
private:
    sf::Keyboard::Key code;
    bool alt, ctrl, shift;
};

class MessageReceiver
{
public:
    virtual ~MessageReceiver();
public:
    void addMessageListener(std::shared_ptr<MessageListener> listener);
    void removeMessageListener(std::shared_ptr<MessageListener> listener);

    void setMessageReceived(bool enable);
    bool isMessageReceived()const;

    virtual bool process(std::shared_ptr<Message> message);
private:
    std::list<std::shared_ptr<MessageListener>> listeners;
    bool active = true;
};

template<class T>
class ProxyMessageListener : public MessageListener
{
public:
    ProxyMessageListener() = delete;
    ProxyMessageListener(std::function<std::shared_ptr<MessageReceiver>()> inputFunction):
        function(inputFunction)
    {
        static_assert (std::is_base_of<MessageReceiver, T>() && "T must be derived of MessageReceiver");
    }
public:
    bool onListener(std::shared_ptr<Message> message)
    {
        auto receiver = function();
        if (receiver)
            receiver->process(message);
        return false;
    }
private:
    std::function<std::shared_ptr<MessageReceiver>()> function;
};
