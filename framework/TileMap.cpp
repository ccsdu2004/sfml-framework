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
        tileSize(size),
        tile(inputTile)
    {
    }

    int32_t type = 0;
    float tileSize = 48.0f;
    Bitmask bitmask;

    void updatePosition(int x, int y)
    {
        if (x % 2 == 0) {
            tile.setPosition(x * (1.5 * tileSize), 2 * 0.86602540f * tileSize * y);
        } else {
            tile.setPosition(x * (1.5 * tileSize), 2.0 * 0.86602540f * tileSize * y + 0.86602540f * tileSize);
        }
    }
private:
    Tile &tile;
};

Tile::Tile(int32_t x, int32_t y, float size):
    data(new TileData(*this, size))
{
    const float s = 1.f * (size);
    const float h = 0.5f * (size);        //sin(30)
    const float r = 0.86602540f * (size); //cos(30)

    setPointCount(6);
    setPoint(0, sf::Vector2f(0.f, -s));
    setPoint(1, sf::Vector2f(-r, -h));
    setPoint(2, sf::Vector2f(-r, +h));
    setPoint(3, sf::Vector2f(0.f, +s));
    setPoint(4, sf::Vector2f(+r, +h));
    setPoint(5, sf::Vector2f(+r, -h));
    rotate(30.0f);

    setVisible(true);
    data->updatePosition(x, y);
}

#define TILE_VISIBLE 0

Tile::~Tile()
{
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

class TileMapImpl
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
    return nullptr;
}

TileMap::TileMap():
    data(new TileMapImpl)
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

    data->tileSize = tilesize;
    data->mapSize.x = width;
    data->mapSize.y = height;

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            auto tile = std::make_shared<Tile>(i, j, tilesize);
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

void TileMap::update(float deltaTime)
{
    (void)deltaTime;
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
