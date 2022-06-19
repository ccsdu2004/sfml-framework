#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <Sprite.h>
#include <SpriteDecorator.h>
#include <Bitmask.h>
#include <iostream>

#define SPRITE_BITMASK_CONTROLLABLE 0

class SpriteData
{
public:
    std::vector<sf::Sprite> sprites;
    std::map<SpriteAttribute, uint32_t> spriteAttribute;
    SpriteStatus spriteStatus;
    sf::Color spriteColor = sf::Color::White;
    SpriteGroupID spriteGroupID;
    SpriteDecoratorPointer spriteDecorator;
    std::weak_ptr<Sprite> spriteOwner;
    Bitmask bitmask;
};

Sprite::Sprite():
    data(new SpriteData())
{
    data->bitmask.setBit(SPRITE_BITMASK_CONTROLLABLE, true);
}

Sprite::~Sprite()
{
}

void Sprite::setSpriteAttribute(SpriteAttribute attribute, uint32_t value)
{
    data->spriteAttribute.insert(std::make_pair(attribute, value));
}

uint32_t Sprite::getSpriteAttribute(SpriteAttribute attribute) const
{
    auto itr = data->spriteAttribute.find(attribute);
    return itr == data->spriteAttribute.end() ? ~0 : itr->second;
}

void Sprite::setSpriteStatus(SpriteStatus status)
{
    if (status != data->spriteStatus) {
        data->spriteStatus = status;
        spriteStatusChanged(std::dynamic_pointer_cast<Sprite>(shared_from_this()));
        onSpriteStatusChanged();
    }
}

SpriteStatus Sprite::getSpriteStatus() const
{
    return data->spriteStatus;
}

void Sprite::setSpriteGroup(SpriteGroupID groupID)
{
    data->spriteGroupID = groupID;
}

SpriteGroupID Sprite::getSpriteGroup() const
{
    return data->spriteGroupID;
}

void Sprite::setControllable(bool controllable)
{
    if (data->bitmask.contain(SPRITE_BITMASK_CONTROLLABLE) != controllable) {
        data->bitmask.setBit(SPRITE_BITMASK_CONTROLLABLE, controllable);
        onControllableChanged();
    }
}

bool Sprite::isControllable()const
{
    return data->bitmask.contain(SPRITE_BITMASK_CONTROLLABLE);
}

void Sprite::setSpriteOwner(SpritePointer owner)
{
    if(owner != data->spriteOwner.lock()) {
        data->spriteOwner = owner;
        onOwnerChanged();
    }
}

SpritePointer Sprite::getSpriteOwner() const
{
    return data->spriteOwner.lock();
}

void Sprite::setSpriteColor(const sf::Color &color)
{
    data->spriteColor = color;
}

sf::Color Sprite::getSpriteColor() const
{
    return data->spriteColor;
}

void Sprite::addTexture(const sf::Texture &texture, const sf::IntRect &area)
{
    sf::Sprite sprite;
    sprite.setTexture(texture);
    if (area.width != 0)
        sprite.setTextureRect(area);
    auto box = sprite.getLocalBounds();
    data->sprites.push_back(sprite);
    setSize(box.width, box.height);
}

void Sprite::setDecorator(SpriteDecoratorPointer spriteDecorator)
{
    data->spriteDecorator = spriteDecorator;
}

void Sprite::clearDecorator()
{
    data->spriteDecorator.reset();
}

void Sprite::onDrawObject(sf::RenderTarget &target, sf::RenderStates states) const
{
    //Entity::onDrawObject(target, states);

    if (data->spriteDecorator)
        target.draw(*data->spriteDecorator.get(), states);

    for (auto itr = data->sprites.begin(); itr != data->sprites.end(); itr++) {
        (*itr).setColor(data->spriteColor);
        target.draw(*itr, states);
    }

    if (data->sprites.empty())
        Entity::onDrawObject(target, states);
}

void Sprite::onSpriteStatusChanged()
{
    if (data->spriteDecorator)
        data->spriteDecorator->update(this);
}

void Sprite::onControllableChanged()
{
    if (data->spriteDecorator)
        data->spriteDecorator->update(this);
}

void Sprite::onPositionChanged()
{
    if (data->spriteDecorator)
        data->spriteDecorator->update(this);
}

void Sprite::onSizeChanged()
{
    if (data->spriteDecorator)
        data->spriteDecorator->update(this);
}

void Sprite::onRotateChanged()
{
    if (data->spriteDecorator)
        data->spriteDecorator->update(this);
}

void Sprite::onScaleChanged()
{

}

void Sprite::onOwnerChanged()
{
}

void SpriteVisitor::visitSprite(SpritePointer sprite)
{
    (void)sprite;
}
