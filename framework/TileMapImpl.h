#pragma once
#include <TileMap.h>

class HexTileMap : public TileMap
{
public:
    std::vector<sf::Vector2i> getAdjacentTileByPosition(int x, int y) override;
    std::optional<sf::Vector2i> getAdjacentTileByDirection(int32_t x, int32_t y, TileDirection direction) override;
    sf::Vector2f getWorldPositionByTileIndex(int x, int y) override;
    sf::Vector2i getTileIndexByWorldPosition(int x, int y) override;
};

class Rect4TileMap : public TileMap
{
public:
    std::vector<sf::Vector2i> getAdjacentTileByPosition(int x, int y) override;
    std::optional<sf::Vector2i> getAdjacentTileByDirection(int32_t x, int32_t y, TileDirection direction) override;
    sf::Vector2f getWorldPositionByTileIndex(int x, int y) override;
    sf::Vector2i getTileIndexByWorldPosition(int x, int y) override;
};


