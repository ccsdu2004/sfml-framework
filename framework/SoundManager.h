#pragma once
#include <memory>
#include <Message.h>
#include <Component.h>

enum SoundMessage_Event {
    SoundMessage_Event_PlaySound = 0,
    SoundMessage_Event_PlayMusic,
    SoundMessage_Event_SetVolume,
    SoundMessage_Event_Stop,
    SoundMessage_Event_Max
};

class SoundMessage : public Message
{
public:
    SoundMessage() = delete;
    explicit SoundMessage(SoundMessage_Event inputEvent):
        Message(Message_SOUND),
        event(inputEvent)
    {
    }

    ~SoundMessage() = default;
public:
    SoundMessage_Event getEvent()const
    {
        return event;
    }
protected:
    SoundMessage_Event event;
};

class SoundMessagePlayBackgroundMusic: public SoundMessage
{
public:
    SoundMessagePlayBackgroundMusic() = delete;
    explicit SoundMessagePlayBackgroundMusic(const std::string &inputMusic):
        SoundMessage(SoundMessage_Event_PlayMusic),
        music(inputMusic)
    {
    }
public:
    std::string getFile()const
    {
        return music;
    }
private:
    std::string music;
};

class SoundMessagePlaySound : public SoundMessage
{
public:
    SoundMessagePlaySound() = delete;
    explicit SoundMessagePlaySound(const std::string &inputSound):
        SoundMessage(SoundMessage_Event_PlaySound),
        sound(inputSound)
    {
    }
public:
    std::string getFile()const
    {
        return sound;
    }
private:
    std::string sound;
};

class SoundMessageSetVolume : public SoundMessage
{
public:
    explicit SoundMessageSetVolume(float inputVolume = 1.0f):
        SoundMessage(SoundMessage_Event_SetVolume),
        volume(inputVolume)
    {
    }
public:
    float getVolume()const
    {
        return volume;
    }
private:
    float volume = 1.0f;
};

class SoundManager : public Component, public MessageReceiver
{
    friend class SoundMessageListener;
public:
    SoundManager();
    ~SoundManager();
public:
    void update(float deltaTime);
private:
    std::unique_ptr<class SoundManagerData> data;
};
