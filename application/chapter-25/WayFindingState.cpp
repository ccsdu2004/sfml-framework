#include "WayFindingTileVisitor.h"
#include "WayFindingState.h"

WayFindingState::WayFindingState(std::shared_ptr<TileMap> inputTileMap, std::shared_ptr<WayFindingTank> inputTank, std::shared_ptr<WayFindingTileVisitor> inputVisitor)
{
    tileMapWeak = inputTileMap;
    tankWeak = inputTank;
    tileVisitor = inputVisitor;
}

void WayFindingState::onEnter()
{
    auto angle = getRotate();
    auto tank = tankWeak.lock();

    if(angle.has_value()) {
        int v = angle.value() - tank->getRotate();
        if(v == 0)
            tank->makeForward();
        else if(v < 0)
            tank->makeTurnLeft();
        else
            tank->makeTurnRight();
    } else {
        auto position = tank->getPosition();
        auto index = tileMapWeak.lock()->getTileIndexByWorldPosition(position.x, position.y);
        tileVisitor.lock()->compute(index);
        onEnter();
    }
}

void WayFindingState::onLeave()
{
}

void WayFindingState::update(float deltaTime)
{
    (void)deltaTime;
    auto tank = tankWeak.lock();
    auto stateName = tank->getCurrentState()->getName();
    if(stateName == "idle") {
        onEnter();
    }
}

std::optional<int> WayFindingState::getRotate()
{
    auto tank = tankWeak.lock();
    auto position = tank->getPosition();

    auto tileMap = tileMapWeak.lock();
    auto index = tileMap->getTileIndexByWorldPosition(position.x, position.y);

    std::optional<int> angle;
    sf::Vector2i tileIndex;

    constexpr auto directions = describe_enumerators_as_array<TileDirection>();
    for(auto const& node : directions) {
        auto current = tileMap->getAdjacentTileByDirection(index.x, index.y, node.value);
        if(current.has_value()) {
            tileIndex = current.value();
            auto tile = tileMap->getTileByIndex(tileIndex);
            if(tile) {
                if(tile->getFillColor() == sf::Color::Blue ||
                   tile->getFillColor() == sf::Color::Green) {
                    angle = node.value;
                    break;
                }
            }
        }
    }
    return angle;
}

WayFindingErrorState::WayFindingErrorState(std::shared_ptr<TileMap> inputTileMap, std::shared_ptr<WayFindingTank> inputTank)
{
    tileMap = inputTileMap;
    tank = inputTank;
}

void WayFindingErrorState::onEnter()
{

}

void WayFindingErrorState::onLeave()
{

}

void WayFindingErrorState::update(float deltaTime)
{
    (void)deltaTime;
}

