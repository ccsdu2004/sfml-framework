#pragma once
#include <vector>
#include <Switch.h>

class Animation final : public Switch
{
public:
    Animation();
    ~Animation();
public:
    void start();
    bool isStarted()const;

    void pause();
    bool isPaused()const;

    void setSingleShot(bool single = true);
    bool getSingleShot()const;

    void setDurationPerFrame(float duration = 200.0f);
    void setTexture(const std::string &image, const std::vector<sf::IntRect> &list);
protected:
    void onUpdateObject(float deltaTime) override;
private:
    std::unique_ptr<class AnimationData> data;
};
