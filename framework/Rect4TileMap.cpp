#include <TileMapImpl.h>

std::optional<sf::Vector2i> Rect4TileMap::getAdjacentTileByDirection(int32_t x, int32_t y, TileDirection direction)
{
    switch (direction) {
        case 0:
            return sf::Vector2i(x, y - 1);
        case 90:
            return sf::Vector2i(x + 1, y);
        case 180:
            return sf::Vector2i(x, y + 1);
        case 270:
            return sf::Vector2i(x - 1, y);
        default:
            break;
    }
    return std::optional<sf::Vector2i>();
}

sf::Vector2f Rect4TileMap::getWorldPositionByTileIndex(int x, int y)
{
    auto tile = getTileByIndex(x, y);
    if (!tile)
        return sf::Vector2f(-1, -1);

    return tile->getPosition();
}

sf::Vector2i Rect4TileMap::getTileIndexByWorldPosition(int x, int y)
{
    x = 1 + (x - getTileSize()) * 0.5 / getTileSize();
    y = 0.5 * (y + getTileSize()) / getTileSize();
    return sf::Vector2i(x, y);
}

std::shared_ptr<Tile> Rect4TileMap::createTile(int32_t i, int32_t j, float tilesize)
{
    return std::make_shared<Tile>(i, j, tilesize, TileMapType_Grid);
}

std::vector<sf::Vector2i> Rect4TileMap::getAdjacentTileByTileIndex(int x, int y)
{
    std::vector<sf::Vector2i> output;
    output.push_back(sf::Vector2i(x, y - 1));
    output.push_back(sf::Vector2i(x, y + 1));
    output.push_back(sf::Vector2i(x - 1, y));
    output.push_back(sf::Vector2i(x + 1, y));
    return output;
}

