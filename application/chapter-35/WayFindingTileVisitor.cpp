#include <Util.h>
#include <Application.h>
#include <Camera.h>
#include "WayFindingTileVisitor.h"
#include <iostream>

WayFindingTileVisitor::WayFindingTileVisitor(TileMapPointer inputTileMap)
{
    weakTileMap = inputTileMap;
    shortestPathFinder = std::make_shared<ShortestPathFinder>();
}

void WayFindingTileVisitor::compute(const sf::Vector2i &inputStart)
{
    start = inputStart;

    blocks.clear();
    tiles.clear();
    shortestPathFinder->clear();
    weakTileMap.lock()->accept(this);

    buildTileMap();

    Application::getInstance()->getComponent<Camera>()->setGlobalArea(tileMapArea);

    std::vector<uint32_t> output;
    int steps = shortestPathFinder->search(getIDByPosition(inputStart.x, inputStart.y),
                                           getIDByPosition(end.x,
                                                   end.y), output);
    if (steps > 0) {
        auto itr = output.begin();
        while (itr != output.end()) {
            auto position = getPositionByID(*itr);
            if (position != start && position != end)
                weakTileMap.lock()->getTileByIndex(position.x, position.y)->setFillColor(PATH_COLOR);
            itr ++;
        }
    } else
        compute(inputStart);
}

void WayFindingTileVisitor::buildTileMap()
{
    auto tileMap = weakTileMap.lock();
    auto startTile = tileMap->getTileByIndex(start.x, start.y);

    std::random_shuffle(tiles.begin(), tiles.end());

    auto itr = tiles.begin();
    end = *itr;

    itr ++;

    const int BLOCKS_COUNT = 400;
    for (int i = 0; i < BLOCKS_COUNT; i++) {
        auto position = *itr;
        auto tile = tileMap->getTileByIndex(position.x, position.y);
        tile->setFillColor(BLOCK_COLOR);
        blocks.push_back(position);
        itr ++;
    }

    startTile = tileMap->getTileByIndex(start);
    if (startTile)
        startTile->setFillColor(sf::Color::Yellow);

    auto endTile = tileMap->getTileByIndex(end);
    if (endTile)
        endTile->setFillColor(sf::Color::Green);
}

void WayFindingTileVisitor::visit(uint32_t x, uint32_t y, std::shared_ptr<Tile> tile)
{
    tile->setFillColor(NORMAL_COLOR);

    if (start != sf::Vector2i(x, y))
        tiles.push_back(sf::Vector2i(x, y));

    auto adjList = weakTileMap.lock()->getAdjacentTileByTileIndex(x, y);
    auto itr = adjList.begin();
    while (itr != adjList.end()) {
        auto adjTile = weakTileMap.lock()->getTileByIndex(itr->x, itr->y);
        if (adjTile && adjTile->getFillColor() != BLOCK_COLOR) {
            shortestPathFinder->addEdge(getIDByPosition(x, y), getIDByPosition(itr->x, itr->y), 1);
        }
        itr ++;
    }

    auto box = tile->getGlobalBounds();
    tileMapArea = expandRect(tileMapArea, box);
}
