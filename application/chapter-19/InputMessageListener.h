#pragma once
#include <Message.h>

class MineSweeperScene;

class InputMessageListener : public MessageListener
{
public:
    InputMessageListener(MineSweeperScene& inputScene);
public:
    bool onListener(std::shared_ptr<Message> message) override;
private:
    MineSweeperScene& scene;
};
