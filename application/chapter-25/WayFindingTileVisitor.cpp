#include "WayFindingTileVisitor.h"

WayFindingTileVisitor::WayFindingTileVisitor(TileMapPointer inputTileMap)
{
    tileMap = inputTileMap;
    shortestPathFinder = std::make_shared<ShortestPathFinder>();
}

void WayFindingTileVisitor::compute(const sf::Vector2i &inputStart)
{
    start = inputStart;

    blocks.clear();
    tiles.clear();
    shortestPathFinder->clear();
    tileMap.lock()->accept(this);

    buildTileMap();

    std::vector<uint32_t> output;
    int steps = shortestPathFinder->search(getIDByPosition(start.x, start.y), getIDByPosition(end.x, end.y), output);
    if(steps > 0) {
        auto itr = output.begin();
        while (itr != output.end()) {
            auto position = getPositionByID(*itr);
            if (position != start && position != end)
                tileMap.lock()->getTileByIndex(position.x, position.y)->setFillColor(PATH_COLOR);
            itr ++;
        }
    } else
        compute(inputStart);
}

void WayFindingTileVisitor::buildTileMap()
{
    auto startTile = tileMap.lock()->getTileByIndex(start.x, start.y);

    std::random_shuffle(tiles.begin(), tiles.end());

    auto itr = tiles.begin();
    end = *itr;

    itr ++;

    const int BLOCKS_COUNT = 100;
    for(int i = 0; i < BLOCKS_COUNT; i++) {
        auto position = *itr;
        auto tile = tileMap.lock()->getTileByIndex(position.x, position.y);
        tile->setFillColor(BLOCK_COLOR);
        blocks.push_back(position);
        itr ++;
    }

    tileMap.lock()->getTileByIndex(start)->setFillColor(sf::Color::Yellow);
    tileMap.lock()->getTileByIndex(end)->setFillColor(sf::Color::Green);
}

void WayFindingTileVisitor::visit(uint32_t x, uint32_t y, std::shared_ptr<Tile> tile)
{
    tile->setFillColor(NORMAL_COLOR);

    if(start != sf::Vector2i(x, y))
        tiles.push_back(sf::Vector2i(x, y));

    auto adjList = tileMap.lock()->getAdjacentTileByPosition(x, y);
    auto itr = adjList.begin();
    while (itr != adjList.end()) {
        auto adjTile = tileMap.lock()->getTileByIndex(itr->x, itr->y);
        if (adjTile && adjTile->getFillColor() != BLOCK_COLOR) {
            shortestPathFinder->addEdge(getIDByPosition(x, y), getIDByPosition(itr->x, itr->y), 1);
        }
        itr ++;
    }
}
