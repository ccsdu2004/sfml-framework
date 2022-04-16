#pragma once
#include <memory>
#include <boost/signals2.hpp>
#include <Entity.h>

class Sprite : public Entity
{
public:
    Sprite();
    virtual ~Sprite();
public:
    void setActive(bool active = true);
    bool isActive()const;

    void setSpriteGroup(SpriteGroupPointer spriteGroup);
    SpriteGroupPointer getSpriteGroup()const;

    boost::signals2::signal<void(SpritePointer)> spriteDeath;

    void setTexture(const sf::Texture &texture);
    void setTextureRect(const sf::IntRect &area);
protected:
    virtual void onPositionChanged() override;
    virtual void onDraw(sf::RenderTarget &target, sf::RenderStates states)const override;
    virtual void onSpriteDeath();
private:
    std::unique_ptr<class SpriteData> data;
};

using SpriteWeakPointer = std::weak_ptr<Sprite>;
