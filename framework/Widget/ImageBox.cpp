#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <Application.h>
#include <Widget/Desktop.h>
#include <Widget/ImageBox.h>

class ImageBoxData
{
public:
    sf::Sprite sprite;
    bool dirty = true;

    std::shared_ptr<WidgetGroup> group;
};

ImageBox::ImageBox(const sf::Vector2f &size):
    Widget(size),
    data(new ImageBoxData())
{
    auto style =
        Application::getInstance()->getComponent<Desktop>()->getWidgetStyle("ImageBoxStyle");
    assert(style);
    setWidgetStyle(style);
}

ImageBox::~ImageBox()
{
}

void ImageBox::setTexture(const sf::Texture &texture)
{
    data->sprite.setTexture(texture);
    data->dirty = true;
}

const sf::Texture *ImageBox::getTexture() const
{
    return data->sprite.getTexture();
}

void ImageBox::setWidgetGroup(std::shared_ptr<WidgetGroup> group)
{
    if(group) {
        group->addWidget(std::dynamic_pointer_cast<Widget>(shared_from_this()));
        data->group = group;
    }
}

void ImageBox::select()
{
    if(!isActive())
        return;

    onMousePressed(sf::Mouse::Button::Left);
}

void ImageBox::unselect()
{
    if(!isActive())
        return;

    data->sprite.setColor(sf::Color::White);

    auto style = std::dynamic_pointer_cast<ImageBoxStyle>(getWidgetStyle());
    if(style->outlineStyle.has_value()) {
        auto outlineStyle = style->outlineStyle.value();
        setOutlineColor(outlineStyle.color);
        setOutlineThickness(outlineStyle.thickness);
    } else {
        setOutlineThickness(0.0f);
    }
}

void ImageBox::onMousePressed(sf::Mouse::Button button)
{
    if(button != sf::Mouse::Left)
        return;

    auto style = std::dynamic_pointer_cast<ImageBoxStyle>(getWidgetStyle());
    data->sprite.setColor(style->pressedColor.value());

    if(style->outlinePressedStyle.has_value()) {
        auto outlineStyle = style->outlinePressedStyle.value();
        setOutlineColor(outlineStyle.color);
        setOutlineThickness(outlineStyle.thickness);
    }

    clicked(std::dynamic_pointer_cast<ImageBox>(shared_from_this()));

    if(data->group)
        data->group->onSelected(std::dynamic_pointer_cast<Widget>(shared_from_this()));
}

void ImageBox::onMouseReleased(sf::Mouse::Button button)
{
    if(button != sf::Mouse::Left)
        return;

    auto style = std::dynamic_pointer_cast<ImageBoxStyle>(getWidgetStyle());

    if(!isRadioMode()) {
        data->sprite.setColor(sf::Color::White);

        if(style->outlineStyle.has_value()) {
            auto outlineStyle = style->outlineStyle.value();
            setOutlineColor(outlineStyle.color);
            setOutlineThickness(outlineStyle.thickness);
        } else {
            setOutlineThickness(0.0f);
        }
    }
}

void ImageBox::onStyleChanged()
{
    Widget::onStyleChanged();

    auto style = std::dynamic_pointer_cast<ImageBoxStyle>(getWidgetStyle());

    data->dirty = true;
}

void ImageBox::onDrawWidget(sf::RenderTarget &target, sf::RenderStates states) const
{
    Widget::onDrawWidget(target, states);
    states.transform = getGlobalTransform();
    target.draw(data->sprite, states);
}

void ImageBox::onPositionChanged()
{
    data->dirty = true;
}

void ImageBox::onSizeChanged()
{
    data->dirty = true;
}

void ImageBox::onScaleChanged()
{
    data->dirty = true;
}

void ImageBox::onUpdateObject(float deltaTime)
{
    Widget::onUpdateObject(deltaTime);
    if (data->dirty) {
        data->dirty = false;

        sf::Vector2u size = data->sprite.getTexture()->getSize();
        float scalex = (float)getSize().x / size.x;
        float scaley = (float)getSize().y / size.y;
        float scale = std::min(scalex, scaley);
        data->sprite.setScale(scale, scale);
    }
}
