#include <map>
#include <cmath>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Text.hpp>
#include <Bitmask.h>
#include <TileMap.h>
#include <TileMapImpl.h>
#include <Application.h>
#include <Util.h>

class TileData : public sf::Vector2i
{
public:
    TileData(Tile &inputTile, float size):
        sideLength(size),
        tile(inputTile)
    {
    }

    float sideLength = 48.0f;
    Bitmask bitmask;

    std::string type;

    void updateHexPosition(int x, int y)
    {
        if (x % 2 == 0) {
            tile.setPosition(x * (1.5 * sideLength), 2 * 0.86602540f * sideLength * y);
        } else {
            tile.setPosition(x * (1.5 * sideLength), 2.0 * 0.86602540f * sideLength * y + 0.86602540f * sideLength);
        }
    }

    void updateGridPosition(int x, int y)
    {
        tile.setPosition(x * sideLength * 2, y * sideLength * 2);
    }

    void updateGridMalPosition(int x, int y)
    {
        if(x % 2 == 0)
            tile.setPosition(x * sideLength * 2, y * sideLength * 2);
        else
            tile.setPosition(x * sideLength * 2, y * sideLength * 2 + sideLength);
    }
private:
    Tile &tile;
};

Tile::Tile(int32_t x, int32_t y, float sideLength, TileMapType type):
    data(new TileData(*this, sideLength))
{
    if(type == TileMapType_Hex) {
        const float s = 1.f * (sideLength);
        const float h = 0.5f * (sideLength);        //sin(30)
        const float r = 0.86602540f * (sideLength); //cos(30)

        setPointCount(6);
        setPoint(0, sf::Vector2f(0.f, -s));
        setPoint(1, sf::Vector2f(-r, -h));
        setPoint(2, sf::Vector2f(-r, +h));
        setPoint(3, sf::Vector2f(0.f, +s));
        setPoint(4, sf::Vector2f(+r, +h));
        setPoint(5, sf::Vector2f(+r, -h));
        rotate(30.0f);
        data->updateHexPosition(x, y);
    } else if(type == TileMapType_Grid) {
        setPointCount(4);
        setPoint(0, sf::Vector2f(-sideLength, -sideLength));
        setPoint(1, sf::Vector2f(sideLength, -sideLength));
        setPoint(2, sf::Vector2f(sideLength, sideLength));
        setPoint(3, sf::Vector2f(-sideLength, sideLength));
        data->updateGridPosition(x, y);
    } else if(type == TileMapType_MalGrid) {
        setPointCount(4);
        setPoint(0, sf::Vector2f(-sideLength, -sideLength));
        setPoint(1, sf::Vector2f(sideLength, -sideLength));
        setPoint(2, sf::Vector2f(sideLength, sideLength));
        setPoint(3, sf::Vector2f(-sideLength, sideLength));
        data->updateGridMalPosition(x, y);
    }

    setVisible(true);
}

#define TILE_VISIBLE 0

Tile::~Tile()
{
}

void Tile::setTileType(const std::string &type)
{
    data->type = type;
}

std::string Tile::getTileType() const
{
    return data->type;
}

void Tile::setVisible(bool visible)
{
    data->bitmask.setBit(TILE_VISIBLE, visible);
}

bool Tile::isVisible()const
{
    return data->bitmask.contain(TILE_VISIBLE);
}

class TileMapMessageListener : public MessageListener
{
public:
    TileMapMessageListener(TileMap &map):
        tileMap(map)
    {
    }
public:
    bool onListener(std::shared_ptr<Message> message)
    {
        if (message->getType() == Message_SFML) {
            auto window = Application::getInstance()->getWindow();
            auto mousePosition = sf::Mouse::getPosition(*window);
            auto position = tileMap.getTileIndexByWorldPosition(mousePosition.x, mousePosition.y);

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) ||
                sf::Mouse::isButtonPressed(sf::Mouse::Middle) ||
                sf::Mouse::isButtonPressed(sf::Mouse::Right))
                tileMap.tileClicked(position.x, position.y);
        }
        return false;
    }
private:
    TileMap &tileMap;
};

class TileMapData
{
public:
    float tileSize = 32.0f;
    sf::Vector2i mapSize = sf::Vector2i(64, 64);
    std::map<int32_t, std::shared_ptr<Tile>> tiles;
    bool textVisible = false;
    std::map<int32_t, std::shared_ptr<sf::Text>> texts;
    std::shared_ptr<sf::Font> font;
};

static const int TILE_ID_OFFSET = 10000;

inline size_t getTileIDByPosition(int x, int y)
{
    return x * TILE_ID_OFFSET + y;
}

inline sf::Vector2i getPositionByID(size_t id)
{
    return sf::Vector2i(id / TILE_ID_OFFSET, id % TILE_ID_OFFSET);
}

std::shared_ptr<TileMap> TileMap::createTileMap(TileMapType type)
{
    if (type == TileMapType_Hex)
        return std::make_shared<HexTileMap>();
    else if(type == TileMapType_Grid)
        return std::make_shared<Rect4TileMap>();
    else if(type == TileMapType_MalGrid)
        return std::make_shared<MalRect4TileMap>();
    return nullptr;
}

TileMap::TileMap():
    data(new TileMapData)
{
    auto listener = std::make_shared<TileMapMessageListener>(*this);
    addMessageListener(listener);
}

TileMap::~TileMap()
{
}

void TileMap::accept(TileVisitor *visitor)
{
    if(!visitor)
        return;

    auto itr = data->tiles.begin();
    while(itr != data->tiles.end()) {
        auto id = getPositionByID(itr->first);
        visitor->visit(id.x, id.y, itr->second);
        itr ++;
    }
}

bool TileMap::init(int width, int height, float tilesize)
{
    data->font = std::make_shared<sf::Font>();
    data->font->loadFromFile("../resource/FZYTK.TTF");

    data->tileSize = tilesize * 0.5f;
    data->mapSize.x = width;
    data->mapSize.y = height;

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            auto tile = createTile(i, j, data->tileSize);
            tile->setOutlineThickness(2.0);
            tile->setFillColor(sf::Color::Black);
            tile->setOutlineColor(sf::Color::Green);
            tile->setOutlineThickness(1.0);
            size_t id = getTileIDByPosition(i, j);
            data->tiles.insert(std::make_pair(id, tile));

            auto text = std::make_shared<sf::Text>();
            text->setFont(*data->font);
            text->setCharacterSize(16);
            text->setFillColor(sf::Color::White);

            std::wstring ss;
            ss = std::to_wstring(i);
            ss += L",";
            ss += std::to_wstring(j);
            text->setString(ss);

            auto box = text->getLocalBounds();
            auto center = tile->getPosition();
            text->setPosition(center.x - box.width * 0.5f, center.y - box.height * 0.5f);
            data->texts.insert(std::make_pair(id, text));
        }
    }

    return true;
}

float TileMap::getTileSize()const
{
    return data->tileSize;
}

sf::Vector2i TileMap::getTileMapSize()const
{
    return data->mapSize;
}

std::shared_ptr<Tile> TileMap::getTileByIndex(const sf::Vector2i &index)
{
    return getTileByIndex(index.x, index.y);
}

std::shared_ptr<Tile> TileMap::getTileByIndex(int x, int y)
{
    auto id = getTileIDByPosition(x, y);
    auto find = data->tiles.find(id);
    return find == data->tiles.end() ? nullptr : find->second;
}

void TileMap::setTextVisible(bool visible)
{
    data->textVisible = visible;
}

bool TileMap::isTextVisible()const
{
    return data->textVisible;
}

void TileMap::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    auto itr = data->tiles.begin();
    while (itr != data->tiles.end()) {
        auto tile = itr->second;
        if (tile && tile->isVisible())
            target.draw(*tile, states);
        itr ++;
    }

    if (data->textVisible) {
        for (auto text : data->texts) {
            target.draw(*text.second, states);
        }
    }

    Object::draw(target, states);
}
