#pragma once
#include <vector>
#include <boost/signals2.hpp>
#include <Switch.h>

class Animation : public Switch
{
public:
    Animation();
    ~Animation();
public:
    void start();
    bool isStarted()const;

    void pause();
    bool isPaused()const;

    boost::signals2::signal<void()> finished;

    void setSingleShot(bool single = true);
    bool getSingleShot()const;

    void setDurationPerFrame(float duration = 200.0f);
    void setTexture(const std::string &image, const std::vector<sf::IntRect> &list);

    bool needRemoved()const override;
protected:
    void onUpdateObject(float deltaTime) override;
private:
    std::unique_ptr<class AnimationData> data;
};
