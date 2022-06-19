#pragma once
#include <vector>
#include <algorithm>
#include <SFML/System/Vector2.hpp>
#include <TileMap.h>
#include <ShortestPathFinder.h>

#define BLOCK_COLOR sf::Color::Red
#define PATH_COLOR sf::Color::Blue
#define NORMAL_COLOR sf::Color::Black

static const int ID_OFFSET = 100;

inline size_t getIDByPosition(int x, int y)
{
    return x * ID_OFFSET + y;
}

inline sf::Vector2i getPositionByID(size_t id)
{
    return sf::Vector2i(id / ID_OFFSET, id % ID_OFFSET);
}

class WayFindingTileVisitor : public TileVisitor
{
public:
    WayFindingTileVisitor() = delete;
    WayFindingTileVisitor(TileMapPointer inputTileMap);
public:
    void compute(const sf::Vector2i& inputStart);
private:
    void buildTileMap();

    void visit(uint32_t x, uint32_t y, std::shared_ptr<Tile> tile) override;
private:
    std::weak_ptr<TileMap> weakTileMap;
    std::shared_ptr<ShortestPathFinder> shortestPathFinder;
    sf::Vector2i start;
    sf::Vector2i end;
    std::vector<sf::Vector2i> blocks;
    std::vector<sf::Vector2i> tiles;
};
