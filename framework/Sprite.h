#pragma once
#include <memory>
#include <boost/signals2.hpp>
#include <boost/describe.hpp>
#include <Entity.h>

class SpriteVisitor
{
public:
    virtual void visitSprite(SpritePointer sprite);
};

enum SpriteAttribute : uint32_t {
    SpriteAttribute_Hp = 0,
    SpriteAttribute_Sp,
    SpriteAttribute_Attack,
    SpriteAttribute_Defence,
    SpriteAttribute_2,
    SpriteAttribute_3,
    SpriteAttribute_Max
};

BOOST_DESCRIBE_ENUM(SpriteAttribute,
                    SpriteAttribute_Hp,
                    SpriteAttribute_Sp,
                    SpriteAttribute_Attack,
                    SpriteAttribute_Defence,
                    SpriteAttribute_2,
                    SpriteAttribute_3,
                    SpriteAttribute_Max)

enum SpriteStatus : uint32_t {
    SpriteStatus_Death = 0,
    SpriteStatus_Normal,
    SpriteStatus_Freeze,
    SpriteStatus_RamPage,
    SpriteStatus_Overwhelming,
    SpriteStatus_2,
    SpriteStatus_3,
    SpriteStatus_Max
};

BOOST_DESCRIBE_ENUM(SpriteStatus,
                    SpriteStatus_Death,
                    SpriteStatus_Normal,
                    SpriteStatus_Freeze,
                    SpriteStatus_RamPage,
                    SpriteStatus_Overwhelming,
                    SpriteStatus_2,
                    SpriteStatus_3,
                    SpriteStatus_Max)

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

    void setSpriteGroup(SpriteGroupID groupID);
    SpriteGroupID getSpriteGroup()const;

    void setControllable(bool controllable);
    bool isControllable()const;

    void setSpriteOwner(SpritePointer owner);
    SpritePointer getSpriteOwner()const;

    boost::signals2::signal<void(SpritePointer)> spriteStatusChanged;
public:
    void setSpriteColor(const sf::Color &color);
    sf::Color getSpriteColor()const;

    void addTexture(const sf::Texture &texture, const sf::IntRect &area = sf::IntRect());

    void setDecorator(SpriteDecoratorPointer spriteDecorator);
    void clearDecorator();
protected:
    virtual void onDrawObject(sf::RenderTarget &target, sf::RenderStates states) const override;
    virtual void onSpriteStatusChanged();
    virtual void onControllableChanged();

    virtual void onPositionChanged() override;
    virtual void onSizeChanged() override;
    virtual void onRotateChanged() override;
    virtual void onScaleChanged() override;

    virtual void onOwnerChanged();
private:
    std::unique_ptr<class SpriteData> data;
};

using SpriteWeakPointer = std::weak_ptr<Sprite>;


