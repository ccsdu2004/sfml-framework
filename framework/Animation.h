#pragma once
#include <Switch.h>

class Animation final : public Switch
{
public:
    Animation();
    ~Animation();
public:
    void setSingleShot(bool single = true);
    bool getSingleShot()const;

    void setDurationPerFrame(float duration = 200.0f);
    void setTexture(const std::string& image, const std::vector<sf::IntRect>& list);
protected:
    void onUpdate(const sf::Time &time) override;
private:
    std::unique_ptr<class AnimationData> data;
};
