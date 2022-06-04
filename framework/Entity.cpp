#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <RoundedRectangleShape.h>
#include <Application.h>
#include <Entity.h>
#include <Bitmask.h>
#include <Util.h>

#define BITMASK_MOUSE_IN        0
#define BITMASK_MOUSE_PRESSED   1
#define BITMASK_MOUSE_RELEASED  2

class EntityImpl
{
public:
    EntityImpl(const std::optional<CornerStyle> &cornerStyle)
    {
        if (cornerStyle.has_value()) {
            auto style = cornerStyle.value();

            roundedRectangle = std::make_shared<sf::RoundedRectangleShape>();
            roundedRectangle->setCornerPointCount(style.pointCount);
            roundedRectangle->setCornersRadius(style.radius);
        } else {
            rectangle = std::make_shared<sf::RectangleShape>();
        }
    }

    std::weak_ptr<Entity> parent;
    std::shared_ptr<sf::RoundedRectangleShape> roundedRectangle;
    std::shared_ptr<sf::RectangleShape> rectangle;
    uint32_t zValue = 0;
    sf::Transformable transform;
    sf::Transform parentTransform;
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

sf::Vector2f Entity::adjustPosition(const sf::FloatRect &box, const sf::Vector2f &target, HMode h,
                                    VMode v, float xmargin, float ymargin)
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

Entity::Entity(const sf::Vector2f &size, const std::optional<CornerStyle> &cornerStyle):
    data(new EntityImpl(cornerStyle))
{
    if (data->rectangle)
        data->rectangle->setSize(size);
    else
        data->roundedRectangle->setSize(size);
    data->transform.setOrigin(size.x * 0.5f, size.y * 0.5f);
    auto listener = std::make_shared<EntityMouseListener>(*this);
    addMessageListener(listener);
}

Entity::~Entity()
{
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

void Entity::move(float distance)
{
    float dx = distance * std::sin(TO_RADIAN * getRotate());
    float dy = distance * std::cos(TO_RADIAN * getRotate());
    move(-dx, dy);
}

void Entity::setRotate(float angle)
{
    if (data->transform.getRotation() != angle) {
        data->transform.setRotation(angle);
        onRotateChanged();
    }
}

float Entity::getRotate() const
{
    return data->transform.getRotation();
}

void Entity::setScale(float scale)
{
    if (scale <= 0.0f)
        return;
    if (isEqual(scale, getScale()))
        return;

    data->transform.setScale(scale, scale);
    onScaleChanged();
}

float Entity::getScale() const
{
    return data->transform.getScale().x;
}

void Entity::setSize(float width, float height)
{
    if (isEqual(width, getSize().x) && isEqual(height, getSize().y))
        return;

    if (data->rectangle)
        data->rectangle->setSize(sf::Vector2f(width, height));
    else
        data->roundedRectangle->setSize(sf::Vector2f(width, height));
    data->transform.setOrigin(width * 0.5f, height * 0.5f);
    onSizeChanged();
    onPositionChanged();
}

sf::Vector2f Entity::getSize() const
{
    if (data->rectangle)
        return data->rectangle->getSize();
    return data->roundedRectangle->getSize();
}

sf::FloatRect Entity::getBoundingBox() const
{
    auto box = data->parentTransform.transformRect(sf::FloatRect(sf::Vector2f(), getSize()));
    return getTransform().transformRect(box);
}

sf::Transform Entity::getTransform() const
{
    return data->transform.getTransform();
}

void Entity::setBackgroundColor(const sf::Color &color)
{
    if (data->rectangle)
        data->rectangle->setFillColor(color);
    else
        data->roundedRectangle->setFillColor(color);
}

sf::Color Entity::getBackgroundColor() const
{
    if (data->rectangle)
        return data->rectangle->getFillColor();
    return data->roundedRectangle->getFillColor();
}

void Entity::setOutlineColor(const sf::Color &color)
{
    if (data->rectangle)
        data->rectangle->setOutlineColor(color);
    else
        data->roundedRectangle->setOutlineColor(color);
}

void Entity::setOutlineThickness(float thickness)
{
    if (data->rectangle)
        data->rectangle->setOutlineThickness(thickness);
    else
        data->roundedRectangle->setOutlineThickness(thickness);
}

void Entity::setZValue(uint32_t z)
{
    data->zValue = z;
}

uint32_t Entity::getZValue() const
{
    return data->zValue;
}

void Entity::onUpdateObject(float deltaTime)
{
    (void)deltaTime;
    if (data->bitmask.contain(BITMASK_MOUSE_IN)) {
        auto mousePosition = sf::Mouse::getPosition(*Application::getInstance()->getWindow());
        auto contain = getBoundingBox().contains(mousePosition.x, mousePosition.y);
        if (!contain) {
            data->bitmask.setBit(BITMASK_MOUSE_IN, false);
            onMouseExit();
        }
    }
}

void Entity::onDrawObject(sf::RenderTarget &target, sf::RenderStates states) const
{
    data->parentTransform = states.transform;

    if (data->rectangle) {
        target.draw(*data->rectangle, states);
    } else
        target.draw(*data->roundedRectangle, states);
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

void Entity::onRotateChanged()
{

}

void Entity::onScaleChanged()
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
            auto position = sf::Mouse::getPosition(
                                *Application::getInstance()->getWindow());

            entity.onMouseMoved(position.x, position.y);
            return true;
        }
    }
    return false;
}
