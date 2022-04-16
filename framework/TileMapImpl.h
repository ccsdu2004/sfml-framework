#pragma once
#include <TileMap.h>

class HexTileMap : public TileMap
{
public:
    std::vector<sf::Vector2i> getAdjacentTileByPosition(int x, int y) override;
    sf::Vector2f getWorldPositionByTileIndex(int x, int y) override;
    sf::Vector2i getTileIndexByWorldPosition(int x, int y) override;
};

class Rect4TileMap : public TileMap
{
public:
    std::vector<sf::Vector2i> getAdjacentTileByPosition(int x, int y) override;
    sf::Vector2f getWorldPositionByTileIndex(int x, int y) override;
    sf::Vector2i getTileIndexByWorldPosition(int x, int y) override;
};


