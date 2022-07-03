#include <cmath>
#include <cassert>
#include <sstream>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <Application.h>
#include <ResourceManager.h>
#include "RadarChart.h"

class RadarAxes : public Entity
{
public:
    RadarAxes();

    void onDrawObject(sf::RenderTarget &target, sf::RenderStates states) const override;
private:
    void drawInnerCirclr(sf::RenderTarget &target, sf::RenderStates states) const;
    void drawOuterCirclr(sf::RenderTarget &target, sf::RenderStates states) const;
    void drawLine(sf::RenderTarget &target, sf::RenderStates states) const;

    void drawLegend(sf::RenderTarget &target, sf::RenderStates states) const;
    void onSizeChanged() override;
public:
    double maxValue = 250 * 0.9;
    sf::Font  font;
    sf::Color fontColor = sf::Color::White;
    sf::Color axesColor = sf::Color::Green;
    sf::Color scaleColor = sf::Color::Blue;

    double radius = 250.0f;
};

RadarAxes::RadarAxes()
{
    auto fontManager = Application::getInstance()->getComponent<ResourceManager<sf::Font>>();
    auto pointer = fontManager->loadFromFile("../resource/accid.ttf");
    assert(pointer);
    font = *pointer;

    setBackgroundColor(sf::Color::Transparent);
}

void RadarAxes::onDrawObject(sf::RenderTarget &target, sf::RenderStates states) const
{
    Entity::onDrawObject(target, states);
    states.transform = getGlobalTransform();
    drawOuterCirclr(target, states);
    drawInnerCirclr(target, states);
    drawLine(target, states);
    if (!font.getInfo().family.empty()) {
        drawLegend(target, states);
    }
}

void RadarAxes::drawInnerCirclr(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (std::size_t i = 1; i < 5; i++) {
        double r = radius / 5.0 * i;
        std::vector<sf::Vertex> vertices;
        for (std::size_t index = 0; index < 31; index++) {
            sf::Vector2f point(radius + r * std::sin(M_PI / 15 * index),
                               radius + r * std::cos(M_PI / 15 * index));
            sf::Vertex vertex(point, scaleColor);
            vertices.push_back(vertex);
        }
        target.draw(&vertices[0], vertices.size(), sf::LineStrip, states);
    }
}

void RadarAxes::drawOuterCirclr(sf::RenderTarget &target, sf::RenderStates states) const
{
    std::vector<sf::Vertex> vertices;
    for (std::size_t index = 0; index < 31; index++) {
        sf::Vector2f point(radius + radius * std::sin(M_PI / 15 * index),
                           radius + radius * std::cos(M_PI / 15 * index));
        sf::Vertex vertex(point, axesColor);
        vertices.push_back(vertex);
    }
    target.draw(&vertices[0], vertices.size(), sf::LineStrip, states);
}

void RadarAxes::drawLine(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (std::size_t index = 0; index < 4; index++) {
        sf::Vertex vertices[2];

        sf::Vector2f start_point(radius + radius * std::sin(M_PI / 4 * index),
                                 radius + radius * std::cos(M_PI / 4 * index));
        vertices[0] = sf::Vertex(start_point, axesColor);

        sf::Vector2f end_point(radius + radius * std::sin(M_PI / 4 * index + M_PI),
                               radius + radius * std::cos(M_PI / 4 * index + M_PI));
        vertices[1] = sf::Vertex(end_point, axesColor);

        target.draw(vertices, 2, sf::Lines, states);
    }
}

void RadarAxes::drawLegend(sf::RenderTarget &target, sf::RenderStates states) const
{
    constexpr double angle_step = 45.0;
    constexpr double MARGIN = 20.0;
    for (int i = 0; i < 8; i++) {
        double angle = angle_step * i;
        sf::Text legend;
        std::stringstream stream;
        stream << angle;
        legend.setString(stream.str());
        legend.setFont(font);
        legend.setFillColor(fontColor);
        legend.setCharacterSize(15);
        legend.setOrigin(legend.getGlobalBounds().width / 2.0, legend.getGlobalBounds().height / 2.0);
        legend.setRotation(angle);
        sf::Vector2f position(radius + (radius + MARGIN) * std::sin(angle / 180.0 * M_PI),
                              radius - (radius + MARGIN) * std::cos(angle / 180.0 * M_PI));
        legend.setPosition(position);
        target.draw(legend, states);
    }

    constexpr double X_MARGIN(20.0);
    constexpr double Y_MARGIN(10.0);
    double value_step = maxValue / 5;

    for (int i = 0; i < 6; i++) {
        double value = value_step * i;
        sf::Text legend;
        std::stringstream stream;
        stream << value;
        legend.setString(stream.str());
        legend.setFont(font);
        legend.setFillColor(fontColor);
        legend.setCharacterSize(15);
        legend.setOrigin(legend.getGlobalBounds().width / 2.0, legend.getGlobalBounds().height / 2.0);
        sf::Vector2f position(radius - X_MARGIN, radius - radius / 5 * i - Y_MARGIN);
        legend.setPosition(position);
        target.draw(legend, states);
    }
}

void RadarAxes::onSizeChanged()
{
    radius = std::max(getSize().x, getSize().y) * 0.5;
}

class RadarChartData
{
public:
    RadarChartData(RadarChart &radarChart):
        chart(radarChart)
    {
    }
public:
    struct Data {
        double angle, value;
        sf::Color color;
    };

    void drawPoint(sf::RenderTarget &target, sf::RenderStates states, const Data &data, double maxValue,
                   double radius) const
    {
        constexpr double POINT_RADIUS(5.0);

        const double r = data.value / maxValue * radius;

        double xoffset = (r + 0.5 * POINT_RADIUS) * std::sin(data.angle / 180.0 * M_PI);
        double yoffset = - (r + 0.5 * POINT_RADIUS) * std::cos(data.angle / 180.0 * M_PI);

        sf::Vector2f position(radius + xoffset - POINT_RADIUS, radius + yoffset - POINT_RADIUS);

        sf::CircleShape point(POINT_RADIUS);
        point.setPosition(position);
        point.setFillColor(data.color);
        target.draw(point, states);
    }

    RadarChart &chart;
    std::vector<Data> datas;
    std::shared_ptr<RadarAxes> radarAxes;
};

RadarChart::RadarChart():
    data(new RadarChartData(*this))
{
    data->radarAxes = std::make_shared<RadarAxes>();
    addChild(data->radarAxes);
    setObjectRenderOrder(ObjectRenderOrder_ChildrenFirst);
    setBackgroundColor(sf::Color::Transparent);
}

RadarChart::~RadarChart()
{
}

void RadarChart::addData(double angle, double value, const sf::Color &color)
{
    data->datas.push_back({angle, value, color});
}

void RadarChart::clear()
{
    data->datas.clear();
}

void RadarChart::autoRange()
{
    auto max = std::numeric_limits<double>::lowest();
    for (const auto &item : data->datas) {
        max = std::max(item.value, max);
    }

    setMaxValue(max);
}

void RadarChart::setMaxValue(const double &value)
{
    data->radarAxes->maxValue = value;
}

void RadarChart::setFont(const std::string file)
{
    auto fontManager = Application::getInstance()->getComponent<ResourceManager<sf::Font>>();
    auto pointer = fontManager->loadFromFile(file);
    if (!pointer)
        return;

    data->radarAxes->font = *pointer;
}

void RadarChart::setFontColor(const sf::Color &color)
{
    data->radarAxes->fontColor = color;
}

sf::Color RadarChart::getFontColor()const
{
    return data->radarAxes->fontColor;
}

void RadarChart::setAxesColor(const sf::Color &color)
{
    data->radarAxes->axesColor = color;
}

sf::Color RadarChart::getAxesColor()
{
    return data->radarAxes->axesColor;
}

void RadarChart::setScaleColor(const sf::Color &color)
{
    data->radarAxes->scaleColor = color;
}

sf::Color RadarChart::getScaleColor() const
{
    return data->radarAxes->scaleColor;
}

void RadarChart::onDrawObject(sf::RenderTarget &target, sf::RenderStates states) const
{
    double radius = std::max(getSize().x, getSize().y) * 0.5;
    states.transform = getGlobalTransform();
    for (const auto &item : data->datas) {
        data->drawPoint(target, states, item, data->radarAxes->maxValue, radius);
    }
}

void RadarChart::onPositionChanged()
{
    data->radarAxes->setPosition(getPosition());
}

void RadarChart::onSizeChanged()
{
    data->radarAxes->setSize(getSize());
}

void RadarChart::onBackgroundChanged()
{
    data->radarAxes->setBackgroundColor(getBackgroundColor());
}

