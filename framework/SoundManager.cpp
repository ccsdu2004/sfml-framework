#include <map>
#include <SFML/Audio.hpp>
#include <SFML/System/Thread.hpp>
#include <SoundManager.h>
#include <Util.h>

class SoundManagerData
{
public:
    bool playBackgroundMusic(std::shared_ptr<SoundMessage> message)
    {
        auto sound = std::dynamic_pointer_cast<SoundMessagePlayBackgroundMusic>(message);
        std::string file = sound->getFile();

        if(!soundBuffer.loadFromFile(file))
            return false;

        backgroundMusic = std::make_shared<sf::Sound>(soundBuffer);
        backgroundMusic->setLoop(true);
        backgroundMusic->play();
        return true;
    }

    bool playSound(std::shared_ptr<SoundMessage> message)
    {
        auto currentMessage = std::dynamic_pointer_cast<SoundMessagePlaySound>(message);
        std::string file = currentMessage->getFile();
        auto sound = std::make_shared<sf::Music>();
        if(!sound->openFromFile(file))
            return false;

        sounds.push_back(sound);
        sound->play();

        return true;
    }

    bool setVolume(std::shared_ptr<SoundMessage> message)
    {
        auto currentMessage = std::dynamic_pointer_cast<SoundMessageSetVolume>(message);
        float volume = currentMessage->getVolume();
        volume = clamp(volume, 0.0f, 1.0f);
        backgroundMusic->setVolume(volume);

        return true;
    }

    bool stopBackgroundMusic()
    {
        backgroundMusic->stop();
        return true;
    }

    void update(float deltaTime)
    {
        (void)deltaTime;
        sounds.remove_if([](std::shared_ptr<sf::Music> music)->bool {
            return music->getStatus() != sf::SoundSource::Playing;
        });
    }
private:
    sf::SoundBuffer soundBuffer;
    std::shared_ptr<sf::Sound> backgroundMusic;

    std::list<std::shared_ptr<sf::Music>> sounds;
};

class SoundMessageListener : public MessageListener
{
public:
    SoundMessageListener(SoundManager &inputManager):
        manager(inputManager)
    {
    }
public:
    bool onListener(std::shared_ptr<Message> message)
    {
        if (message->getType() != Message_SOUND)
            return false;

        auto soundMessage = std::dynamic_pointer_cast<SoundMessage>(message);
        if (soundMessage->getEvent() == SoundMessage_Event_PlaySound)
            return manager.data->playSound(soundMessage);
        else if (soundMessage->getEvent() == SoundMessage_Event_PlayMusic)
            return manager.data->playBackgroundMusic(soundMessage);
        else if (soundMessage->getEvent() == SoundMessage_Event_SetVolume)
            return manager.data->setVolume(soundMessage);
        else if (soundMessage->getEvent() == SoundMessage_Event_Stop)
            return manager.data->stopBackgroundMusic();
        return false;
    }
private:
    SoundManager &manager;
};

SoundManager::SoundManager():
    data(new SoundManagerData())
{
    auto listener = std::make_shared<SoundMessageListener>(*this);
    addMessageListener(listener);
}

SoundManager::~SoundManager()
{
}

void SoundManager::update(float deltaTime)
{
    data->update(deltaTime);
}
