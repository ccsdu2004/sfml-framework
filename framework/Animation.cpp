#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <Application.h>
#include <Sprite.h>
#include <Animation.h>
#include <iostream>

class AnimationData
{
public:
    sf::Sprite sprite;
    bool singleShot = true;
    float duration = 120.0f;
    float currentTime = 0.0f;

    std::shared_ptr<Sprite> createSprite(const sf::Texture& texture, const sf::IntRect& area)
    {
        auto sprite = std::make_shared<Sprite>();
        sprite->setTexture(texture);
        sprite->setTextureRect(area);
        sprite->setSize(texture.getSize().x, texture.getSize().y);
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
        addEntity(sprite);
        itr ++;
    }
}

void Animation::onUpdate(const sf::Time &time)
{
    data->currentTime += time.asMilliseconds();
    if(data->currentTime > data->duration) {
        data->currentTime = 0.0f;
        next();
    }

    if(getCurrent() > getEntityCount() && !data->singleShot)
        setCurrent(0);
}




