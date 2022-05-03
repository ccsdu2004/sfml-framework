#include <cmath>
#include <TileMapImpl.h>
#include <Util.h>

#define R3 (1.73205)

std::vector<sf::Vector2i> HexTileMap::getAdjacentTileByPosition(int x, int y)
{
    std::vector<sf::Vector2i> output;

    output.push_back(sf::Vector2i(x, y - 1));
    output.push_back(sf::Vector2i(x, y + 1));
    output.push_back(sf::Vector2i(x - 1, y));
    output.push_back(sf::Vector2i(x + 1, y));

    if (x % 2 == 0) {
        output.push_back(sf::Vector2i(x - 1, y - 1));
        output.push_back(sf::Vector2i(x + 1, y - 1));
    } else {
        output.push_back(sf::Vector2i(x - 1, y + 1));
        output.push_back(sf::Vector2i(x + 1, y + 1));
    }
    return output;
}

std::optional<sf::Vector2i> HexTileMap::getAdjacentTileByDirection(int32_t x, int32_t y, TileDirection direction)
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
                return sf::Vector2i(x - 1, y + 1);
            case 300:
                return sf::Vector2i(x - 1, y);
            default:
                break;
        }
    }

    return std::optional<sf::Vector2i>();
}

sf::Vector2f HexTileMap::getWorldPositionByTileIndex(int x, int y)
{
    auto tile = getTileByIndex(x, y);
    if (!tile)
        return sf::Vector2f(-1, -1);

    return tile->getPosition();
}

sf::Vector2i HexTileMap::getTileIndexByWorldPosition(int x, int y)
{
    sf::Vector2i point(-1, -1);
    float xpos = x - getTileSize() * 0.5f;
    float ypos = y / (R3 * getTileSize()) - 0.5f;
    float yset[2] = {std::floor(ypos), std::ceil(ypos)};
    xpos /= (1.5 * getTileSize());
    float xset[2] = {std::floor(xpos), std::ceil(xpos)};
    auto p00 = getWorldPositionByTileIndex(xset[0], yset[0]);
    auto p01 = getWorldPositionByTileIndex(xset[0], yset[1]);
    auto p10 = getWorldPositionByTileIndex(xset[1], yset[0]);
    auto p11 = getWorldPositionByTileIndex(xset[1], yset[1]);
    float d00 = distance2<float>(x, y, p00.x, p00.y);
    float d01 = distance2<float>(x, y, p01.x, p01.y);
    float d10 = distance2<float>(x, y, p10.x, p10.y);
    float d11 = distance2<float>(x, y, p11.x, p11.y);

    int i, j;
    if (d00 < d01 && d00 < d10 && d00 < d11) {
        i = xset[0];
        j = yset[0];
    } else if (d00 > d01 && d01 < d10 && d01 < d11) {
        i = xset[0];
        j = yset[1];
    } else if (d10 < d00 && d10 < d01 && d10 < d11) {
        i = xset[1];
        j = yset[0];
    } else {
        i = xset[1];
        j = yset[1];
    }

    return sf::Vector2i(i, j);
}

std::vector<sf::Vector2i> Rect4TileMap::getAdjacentTileByPosition(int x, int y)
{
    std::vector<sf::Vector2i> output;
    output.push_back(sf::Vector2i(x, y - 1));
    output.push_back(sf::Vector2i(x, y + 1));
    output.push_back(sf::Vector2i(x - 1, y));
    output.push_back(sf::Vector2i(x + 1, y));
    return output;
}

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
    return sf::Vector2i(x / getTileSize(), y / getTileSize());
}

