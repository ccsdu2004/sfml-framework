#pragma once
#include <vector>
#include <Entity.h>

class RadarChart : public Entity
{
public:
    RadarChart();
    ~RadarChart();
public:
    void addData(double angle, double value, const sf::Color& color = sf::Color::Red);
    void clear();

    void autoRange();
    void setMaxValue(const double& max);

    void setFont(const std::string file);

    void setFontColor(const sf::Color& color);
    sf::Color getFontColor()const;

    void setAxesColor(const sf::Color& color);
    sf::Color getAxesColor();

    void setScaleColor(const sf::Color& color);
    sf::Color getScaleColor() const;
protected:
    void onDrawObject(sf::RenderTarget &target, sf::RenderStates states) const override;
    void onPositionChanged()override;
    void onSizeChanged()override;

    void onBackgroundChanged()override;
private:
    std::unique_ptr<class RadarChartData> data;
};
