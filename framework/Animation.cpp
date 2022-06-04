#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <Application.h>
#include <Sprite.h>
#include <Animation.h>

class AnimationData
{
public:
    bool started = false;
    bool singleShot = true;
    float duration = 120.0f;
    float currentTime = 0.0f;
    bool needRemoved = false;

    std::shared_ptr<Sprite> createSprite(const sf::Texture& texture, const sf::IntRect& area)
    {
        auto sprite = std::make_shared<Sprite>();
        sprite->addTexture(texture, area);
        sprite->setSize(area.width, area.height);
        return sprite;
    }
};

Animation::Animation():
    data(new AnimationData())
{
}

Animation::~Animation()
{
}

void Animation::start()
{
    data->started = true;
}

bool Animation::isStarted() const
{
    return data->started;
}

void Animation::pause()
{
    data->started = false;
}

bool Animation::isPaused() const
{
    return !data->started;
}

void Animation::setSingleShot(bool single)
{
    data->singleShot = single;
}

bool Animation::getSingleShot() const
{
    return data->singleShot;
}

void Animation::setDurationPerFrame(float duration)
{
    data->duration = duration;
}

void Animation::setTexture(const std::string &image, const std::vector<sf::IntRect> &list)
{
    clear();

    auto texture = Application::getInstance()->loadTexture(image);
    if(!texture)
        return;

    auto itr = list.begin();
    while(itr != list.end()) {
        auto sprite = data->createSprite(*texture, *itr);
        sprite->setSpriteColor(getBackgroundColor());
        addItem(sprite);
        itr ++;
    }
}

bool Animation::needRemoved() const
{
    return data->needRemoved;
}

void Animation::onUpdateObject(float deltaTime)
{
    if(isPaused())
        return;

    data->currentTime += deltaTime;

    int current = getCurrentItem();
    if(current < 0)
        setCurrentItem(0);

    if(data->currentTime > data->duration) {
        data->currentTime = 0.0f;
        next();
    }

    if(current > getItemCount() && !data->singleShot)
        setCurrentItem(0);
    else if(current > getItemCount()) {
        finished();
        data->needRemoved = true;
    }
}
