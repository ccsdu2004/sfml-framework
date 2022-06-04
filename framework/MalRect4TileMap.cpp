#include <TileMapImpl.h>

std::vector<sf::Vector2i> MalRect4TileMap::getAdjacentTileByTileIndex(int x, int y)
{
    std::vector<sf::Vector2i> output;
    if(x % 2 == 0) {
        output.push_back(sf::Vector2i(x - 1, y - 1));
        output.push_back(sf::Vector2i(x, y - 1));
        output.push_back(sf::Vector2i(x, y + 1));
        output.push_back(sf::Vector2i(x - 1, y));
        output.push_back(sf::Vector2i(x + 1, y));
        output.push_back(sf::Vector2i(x + 1, y - 1));
    } else {
        output.push_back(sf::Vector2i(x - 1, y + 1));
        output.push_back(sf::Vector2i(x, y - 1));
        output.push_back(sf::Vector2i(x, y + 1));
        output.push_back(sf::Vector2i(x - 1, y));
        output.push_back(sf::Vector2i(x + 1, y + 1));
        output.push_back(sf::Vector2i(x + 1, y));
    }
    return output;
}

std::optional<sf::Vector2i> MalRect4TileMap::getAdjacentTileByDirection(int32_t x, int32_t y, TileDirection direction)
{
    if(x % 2 == 0) {
        switch (direction) {
            case 0:
                return sf::Vector2i(x, y - 1);
            case 60:
                return sf::Vector2i(x + 1, y - 1);
            case 120:
                return sf::Vector2i(x + 1, y);
            case 180:
                return sf::Vector2i(x, y + 1);
            case 240:
                return sf::Vector2i(x - 1, y);
            case 300:
                return sf::Vector2i(x - 1, y - 1);
            default:
                break;
        }
    } else {
        switch (direction) {
            case 0:
                return sf::Vector2i(x, y - 1);
            case 60:
                return sf::Vector2i(x + 1, y);
            case 120:
                return sf::Vector2i(x + 1, y + 1);
            case 180:
                return sf::Vector2i(x, y + 1);
            case 240:
                return sf::Vector2i(x - 1, y);
            case 300:
                return sf::Vector2i(x - 1, y + 1);
            default:
                break;
        }
    }
    return std::optional<sf::Vector2i>();
}

sf::Vector2f MalRect4TileMap::getWorldPositionByTileIndex(int x, int y)
{
    auto tile = getTileByIndex(x, y);
    if (!tile)
        return sf::Vector2f(-1, -1);

    return tile->getPosition();
}

sf::Vector2i MalRect4TileMap::getTileIndexByWorldPosition(int x, int y)
{
    x = 1 + (x - getTileSize()) * 0.5 / getTileSize();
    if(x % 2 == 0)
        y = 0.5 * (y + getTileSize()) / getTileSize();
    else
        y = 0.5 * (y - 2 * getTileSize()) / getTileSize() + 1;
    return sf::Vector2i(x, y);
}

std::shared_ptr<Tile> MalRect4TileMap::createTile(int32_t i, int32_t j, float tilesize)
{
    return std::make_shared<Tile>(i, j, tilesize, TileMapType_MalGrid);
}

