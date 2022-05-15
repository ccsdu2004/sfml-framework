#pragma once
#include <optional>
#include <memory>
#include <boost/signals2.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <Object.h>
#include <Component.h>

namespace std
{
template<>
struct hash<sf::Vector2i> {
    size_t operator()(const sf::Vector2i &vec) const
    {
        hash<int> int_hash;
        size_t hq = int_hash(vec.x);
        size_t hr = int_hash(vec.y);
        return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
    }
};
}

class Tile : public sf::ConvexShape
{
public:
    Tile() = delete;
    Tile(int32_t x, int32_t y, float size = 48.0f);
    ~Tile();
public:
    void setVisible(bool visible);
    bool isVisible()const;
private:
    std::unique_ptr<class TileData> data;
};

class TileVisitor
{
public:
    virtual void visit(uint32_t x, uint32_t y, std::shared_ptr<Tile> tile) = 0;
};

enum TileMapType {
    TileMapType_Hex = 0,
    TileMapType_Grid
};

enum TileDirection {
    TileDirection_0 = 0,
    TileDirection_60 = 60,
    TileDirection_90 = 90,
    TileDirection_120 = 120,
    TileDirection_180 = 180,
    TileDirection_240 = 240,
    TileDirection_270 = 270,
    TileDirection_300 = 300,
    TileDirection_Max,
};

class TileMap : public Object, public Component
{
public:
    static std::shared_ptr<TileMap> createTileMap(TileMapType type);

    TileMap();
    virtual ~TileMap();
public:
    bool init(int32_t width, int32_t height, float tilesize);

    void accept(TileVisitor *visitor);

    float getTileSize()const;
    sf::Vector2i getTileMapSize()const;
    std::shared_ptr<Tile> getTileByIndex(int32_t x, int32_t y);

    void setTextVisible(bool visible);
    bool isTextVisible()const;
public:
    virtual std::vector<sf::Vector2i> getAdjacentTileByPosition(int32_t x, int32_t y) = 0;
    virtual std::optional<sf::Vector2i> getAdjacentTileByDirection(int32_t x, int32_t y, TileDirection direction) = 0;
    virtual sf::Vector2f getWorldPositionByTileIndex(int32_t x, int32_t y) = 0;
    virtual sf::Vector2i getTileIndexByWorldPosition(int32_t x, int32_t y) = 0;

    boost::signals2::signal<void(int32_t, int32_t)> tileClicked;
protected:
    void update(float deltaTime)override;
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
protected:
    std::unique_ptr<class TileMapImpl> data;
};

using TileMapPointer = std::shared_ptr<TileMap>;
