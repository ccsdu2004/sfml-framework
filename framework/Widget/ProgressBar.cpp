#include <SFML/Graphics/RenderTarget.hpp>
#include <RoundedRectangleShape.h>
#include <Application.h>
#include <ResourceManager.h>
#include <Widget/Desktop.h>
#include <Widget/ProgressBar.h>

class ProgressBarData
{
public:
    Orientation orientation = Orientation_H;
    sf::RoundedRectangleShape progressBarShape;
    bool textVisible = true;
    sf::Color textColor = sf::Color::White;
    std::shared_ptr<sf::Text> text;
};

ProgressBar::ProgressBar():
    data(new ProgressBarData())
{
    data->progressBarShape.setCornersRadius(3.0f);
    data->progressBarShape.setCornerPointCount(16);
    data->text = std::make_shared<sf::Text>();

    setHintSize(sf::Vector2f(480.0f, 12.0f));

    auto style = Application::getInstance()->getComponent<Desktop>()->getWidgetStyle("ProgressBarStyle");
    assert(style);
    setWidgetStyle(style);
}

ProgressBar::~ProgressBar()
{
}

void ProgressBar::setTextVisible(bool visible)
{
    data->textVisible = visible;
}

bool ProgressBar::isTextVisible() const
{
    return data->textVisible;
}

void ProgressBar::setOrientation(Orientation o)
{
    if(data->orientation != o) {
        data->orientation = o;
        onValueChanged();
    }
}

Orientation ProgressBar::getOrientation()const
{
    return data->orientation;
}

void ProgressBar::onStyleChanged()
{
    AbstractSlider::onStyleChanged();

    auto style = std::dynamic_pointer_cast<ProgressBarStyle>(getWidgetStyle());

    data->progressBarShape.setFillColor(style->progressBarColor);

    auto fontManager = Application::getInstance()->getComponent<ResourceManager<sf::Font>>();
    auto font = fontManager->loadFromFile(style->font);
    data->text->setFont(*font);
    data->text->setCharacterSize(style->size);
    data->text->setStyle(style->style);
    data->text->setFillColor(style->textColor);
    data->text->setPosition(getSize() * 0.5f);

    onPositionChanged();
}

void ProgressBar::onDrawWidget(sf::RenderTarget &target, sf::RenderStates states) const
{
    AbstractSlider::onDrawWidget(target, states);
    states.transform = getGlobalTransform();
    target.draw(data->progressBarShape, states);
    if(data->textVisible)
        target.draw(*data->text, states);
}

void ProgressBar::onPositionChanged()
{
    onValueChanged();
}

void ProgressBar::onValueChanged()
{
    if(data->orientation == Orientation_H) {
        auto width = getWidth() * getValuePercent();
        data->progressBarShape.setSize(sf::Vector2f(width, getHeight()));
    } else {
        auto height = getHeight() * getValuePercent();
        data->progressBarShape.setSize(sf::Vector2f(getWidth(), height));
    }

    data->text->setString(std::to_string(getValue()) + "%");

    auto size = getSize();
    size.x = data->text->getGlobalBounds().width;
    size.y = data->text->getGlobalBounds().height;

    size = Entity::adjustPosition(sf::FloatRect(sf::Vector2f(), getSize()), size, HMode_Center, VMode_Center);
    size.y -= getPadding();
    data->text->setPosition(size);
}

