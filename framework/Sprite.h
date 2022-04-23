#pragma once
#include <memory>
#include <boost/signals2.hpp>
#include <Entity.h>

enum SpriteAttribute : uint32_t {
    SpriteAttribute_Hp = 0,
    SpriteAttribute_Sp,
    SpriteAttribute_Attack,
    SpriteAttribute_Defence,
    SpriteAttribute_2,
    SpriteAttribute_3,
    SpriteAttribute_4,
    SpriteAttribute_5,
    SpriteAttribute_Max
};

enum SpriteStatus : uint32_t {
    SpriteStatus_Death = 0,
    SpriteStatus_Normal,
    SpriteStatus_Freeze,
    SpriteStatus_RamPage,
    SpriteStatus_Overwhelming,
    SpriteStatus_2,
    SpriteStatus_3,
    SpriteStatus_4,
    SpriteStatus_5,
    SpriteStatus_Max
};

class Sprite : public Entity
{
public:
    Sprite();
    virtual ~Sprite();
public:
    void setSpriteAttribute(SpriteAttribute attribute, uint32_t value);
    uint32_t getSpriteAttribute(SpriteAttribute attribute)const;

    void setSpriteStatus(SpriteStatus status);
    SpriteStatus getSpriteStatus()const;

    boost::signals2::signal<void(SpritePointer)> spriteStatusChanged;
public:
    void setSpriteGroup(SpriteGroupPointer spriteGroup);
    SpriteGroupPointer getSpriteGroup()const;

    void setSpriteColor(const sf::Color &color);
    sf::Color getSpriteColor()const;

    void addTexture(const sf::Texture &texture, const sf::IntRect &area = sf::IntRect());
protected:
    virtual void onDrawObject(sf::RenderTarget &target, sf::RenderStates states) const override;
    virtual void onSpriteStatusChanged();
private:
    std::unique_ptr<class SpriteData> data;
};

using SpriteWeakPointer = std::weak_ptr<Sprite>;
