#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <Sprite.h>

class SpriteData
{
public:
    sf::Sprite sprite;
    bool active = true;
    SpriteGroupPointer spriteGroup;
};

Sprite::Sprite():
    data(new SpriteData())
{
}

Sprite::~Sprite()
{
}

void Sprite::setActive(bool active)
{
    if(active != data->active && data->active) {
        onSpriteDeath();
        spriteDeath(std::dynamic_pointer_cast<Sprite>(shared_from_this()));
    }
    data->active = active;
}

bool Sprite::isActive() const
{
    return data->active;
}

void Sprite::setSpriteGroup(SpriteGroupPointer spriteGroup)
{
    data->spriteGroup = spriteGroup;
}

SpriteGroupPointer Sprite::getSpriteGroup() const
{
    return data->spriteGroup;
}

void Sprite::setTexture(const sf::Texture &texture)
{
    data->sprite.setTexture(texture);
    auto box = data->sprite.getLocalBounds();
    setSize(box.width, box.height);
}

void Sprite::setTextureRect(const sf::IntRect &area)
{
    data->sprite.setTextureRect(area);
}

void Sprite::onPositionChanged()
{
    data->sprite.setPosition(getPosition());
}

void Sprite::onDraw(sf::RenderTarget &target, sf::RenderStates states)const
{
    target.draw(data->sprite, states);
}

void Sprite::onSpriteDeath()
{

}


