#pragma once
#include <Message.h>

class HelpListener : public MessageListener
{
public:
    HelpListener() = delete;
    HelpListener(const sf::String& text,const sf::Vector2f& size);
    ~HelpListener();
public:
    bool onListener(std::shared_ptr<Message> message) override;
private:
    std::unique_ptr<class HelpListenerData> data;
};
