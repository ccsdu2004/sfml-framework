#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <RoundedRectangleShape.h>
#include <Application.h>
#include <Entity.h>
#include <Bitmask.h>
#include <iostream>

#define BITMASK_MOUSE_IN        0
#define BITMASK_MOUSE_PRESSED   1
#define BITMASK_MOUSE_RELEASED  2

class EntityImpl
{
public:
    EntityImpl()
    {
        rectange.setCornerPointCount(12);
        rectange.setCornersRadius(3.0f);
    }

    std::weak_ptr<Entity> parent;
    sf::RoundedRectangleShape rectange;
    sf::Transformable transform;
    Bitmask bitmask;
};

class EntityMouseListener : public MessageListener
{
public:
    EntityMouseListener(Entity &e):
        entity(e)
    {
    }

    bool onListener(std::shared_ptr<Message> message);
private:
    Entity &entity;
};

sf::Vector2f Entity::adjustPosition(const sf::FloatRect &box, const sf::Vector2f &target, HMode h, VMode v, float xmargin, float ymargin)
{
    sf::Vector2f ret;
    if (h == HMode::HMode_Left)
        ret.x = box.left + xmargin;
    else if (h == HMode::HMode_Right)
        ret.x = box.left + box.width - target.x - xmargin;
    else
        ret.x = box.left + (box.width - target.x) * 0.5f;

    if (v == VMode::VMode_Top)
        ret.y = box.top + ymargin;
    else if (v == VMode::VMode_Bottom)
        ret.y = box.top + box.height - target.y - ymargin;
    else if (v == VMode::VMode_Center)
        ret.y = box.top + (box.height - target.y) * 0.5f;
    return ret;
}

Entity::Entity(const sf::Vector2f &size):
    data(new EntityImpl())
{
    data->rectange.setSize(size);
    data->transform.setOrigin(size.x * 0.5f, size.y * 0.5f);
    auto listener = std::make_shared<EntityMouseListener>(*this);
    addMessageListener(listener);
}

Entity::~Entity()
{
}

void Entity::setParent(std::shared_ptr<Entity> parent)
{
    data->parent = parent;
}

std::shared_ptr<Entity> Entity::getParent() const
{
    return data->parent.lock();
}

void Entity::setPosition(float x, float y)
{
    data->transform.setPosition(x, y);
    onPositionChanged();
}

void Entity::setPosition(const sf::Vector2f &position)
{
    setPosition(position.x, position.y);
}

sf::Vector2f Entity::getPosition() const
{
    return data->transform.getPosition();
}

void Entity::move(float dx, float dy)
{
    auto position = getPosition();
    setPosition(position.x + dx, position.y + dy);
}

void Entity::setRotate(float angle)
{
    data->transform.setRotation(angle);
}

void Entity::setSize(float width, float height)
{
    data->rectange.setSize(sf::Vector2f(width, height));
    data->transform.setOrigin(width * 0.5f, height * 0.5f);
    onSizeChanged();
}

sf::Vector2f Entity::getSize() const
{
    return data->rectange.getSize();
}

sf::Vector2f Entity::getCenter() const
{
    auto box = getBoundingBox();
    return sf::Vector2f(box.left + box.width * 0.5f, box.top + box.height * 0.5f);
}

sf::FloatRect Entity::getBoundingBox() const
{
    return data->transform.getTransform().transformRect(sf::FloatRect(sf::Vector2f(), getSize()));
}

sf::Transform Entity::getTransform() const
{
    return data->transform.getTransform();
}

void Entity::setBackgroundColor(const sf::Color &color)
{
    data->rectange.setFillColor(color);
}

sf::Color Entity::getBackgroundColor() const
{
    return data->rectange.getFillColor();
}

void Entity::setOutlineColor(const sf::Color &color)
{
    data->rectange.setOutlineColor(color);
}

void Entity::setOutlineThickness(float thickness)
{
    data->rectange.setOutlineThickness(thickness);
}

void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= data->transform.getTransform();
    onDraw(target, states);
}

void Entity::update(const sf::Time &time)
{
    if (data->bitmask.contain(BITMASK_MOUSE_IN)) {
        auto mousePosition = sf::Mouse::getPosition(*Application::getInstance()->getWindow());
        auto contain = getBoundingBox().contains(mousePosition.x, mousePosition.y);
        if (!contain) {
            data->bitmask.setBit(BITMASK_MOUSE_IN, false);
            onMouseExit();
        }
    }

    onUpdate(time);
}

void Entity::onUpdate(const sf::Time &time)
{
    (void)time;
}

void Entity::onDraw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(data->rectange, states);
}

void Entity::onMouseEnter()
{
}

void Entity::onMouseExit()
{
}

void Entity::onMousePressed(sf::Mouse::Button button)
{
    (void)button;
}

void Entity::onMouseReleased(sf::Mouse::Button button)
{
    (void)button;
}

void Entity::onMouseWheelScroll(float scroll)
{
    (void)scroll;
}

void Entity::onMouseMoved(int x, int y)
{
    (void)x, (void)y;
}

void Entity::onPositionChanged()
{
}

void Entity::onSizeChanged()
{
}

bool EntityMouseListener::onListener(std::shared_ptr<Message> message)
{
    if (message->getType() != Message_SFML)
        return false;

    auto sfmlMessage = std::dynamic_pointer_cast<SFMLMessage>(message);

    auto mousePosition = sf::Mouse::getPosition(*Application::getInstance()->getWindow());
    auto contain = entity.getBoundingBox().contains(mousePosition.x, mousePosition.y);

    if (contain) {
        if (!entity.data->bitmask.contain(BITMASK_MOUSE_IN)) {
            entity.data->bitmask.setBit(BITMASK_MOUSE_IN, true);
            entity.onMouseEnter();
            return true;
        }
    } else {
        if (entity.data->bitmask.contain(BITMASK_MOUSE_IN)) {
            entity.data->bitmask.setBit(BITMASK_MOUSE_IN, false);
            entity.onMouseExit();
            return true;
        }
    }

    if (entity.data->bitmask.contain(BITMASK_MOUSE_IN)) {
        if (sfmlMessage->getEvent().type == sf::Event::MouseButtonPressed) {
            entity.onMousePressed(sfmlMessage->getEvent().mouseButton.button);
            return true;
        } else if (sfmlMessage->getEvent().type == sf::Event::MouseButtonReleased) {
            entity.onMouseReleased(sfmlMessage->getEvent().mouseButton.button);
            return true;
        } else if (sfmlMessage->getEvent().type == sf::Event::MouseWheelScrolled) {
            entity.onMouseWheelScroll(sfmlMessage->getEvent().mouseWheelScroll.delta);
            return true;
        } else if (sfmlMessage->getEvent().type == sf::Event::MouseMoved) {
            auto button = sfmlMessage->getEvent().mouseButton;
            entity.onMouseMoved(button.x, button.y);
            return true;
        }
    }
    return false;
}
