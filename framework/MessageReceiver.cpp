#include <Message.h>

MessageReceiver::~MessageReceiver()
{
}

void MessageReceiver::addMessageListener(std::shared_ptr<MessageListener> listener)
{
    if (listener)
        listeners.push_back(listener);
}

void MessageReceiver::removeMessageListener(std::shared_ptr<MessageListener> listener)
{
    listeners.remove(listener);
}

void MessageReceiver::setMessageReceived(bool enable)
{
    active = enable;
}

bool MessageReceiver::isMessageReceived() const
{
    return active;
}

bool MessageReceiver::process(std::shared_ptr<Message> message)
{
    if (!isMessageReceived())
        return false;

    auto itr = listeners.begin();
    while (itr != listeners.end()) {
        auto listener = *itr;
        if (listener->onListener(message))
            return true;
        itr ++;
    }
    return false;
}
