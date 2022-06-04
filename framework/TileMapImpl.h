#pragma once
#include <TileMap.h>

class HexTileMap : public TileMap
{
public:
    std::vector<sf::Vector2i> getAdjacentTileByTileIndex(int x, int y) override;
    std::optional<sf::Vector2i> getAdjacentTileByDirection(int32_t x, int32_t y, TileDirection direction) override;
    sf::Vector2f getWorldPositionByTileIndex(int x, int y) override;
    sf::Vector2i getTileIndexByWorldPosition(int x, int y) override;
private:
    std::shared_ptr<Tile> createTile(int32_t i, int32_t j, float tilesize)override;
};

class Rect4TileMap : public TileMap
{
public:
    std::vector<sf::Vector2i> getAdjacentTileByTileIndex(int x, int y) override;
    std::optional<sf::Vector2i> getAdjacentTileByDirection(int32_t x, int32_t y, TileDirection direction) override;
    sf::Vector2f getWorldPositionByTileIndex(int x, int y) override;
    sf::Vector2i getTileIndexByWorldPosition(int x, int y) override;
private:
    std::shared_ptr<Tile> createTile(int32_t i, int32_t j, float tilesize) override;
};

class MalRect4TileMap : public TileMap
{
public:
    std::vector<sf::Vector2i> getAdjacentTileByTileIndex(int x, int y) override;
    std::optional<sf::Vector2i> getAdjacentTileByDirection(int32_t x, int32_t y, TileDirection direction) override;
    sf::Vector2f getWorldPositionByTileIndex(int x, int y) override;
    sf::Vector2i getTileIndexByWorldPosition(int x, int y) override;
private:
    std::shared_ptr<Tile> createTile(int32_t i, int32_t j, float tilesize) override;
};

