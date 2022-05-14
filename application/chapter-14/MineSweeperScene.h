#pragma once
#include <StateMachine.h>
#include <QuadTreeScene.h>
#include <TileMap.h>
#include <Text.h>
#include <MovingSprite.h>

static inline const int TILE_ID_OFFSET = 10000;

inline size_t getTileIDByPosition(int x, int y)
{
    return x * TILE_ID_OFFSET + y;
}

class MineSweeperScene : public QuadTreeScene, public TileVisitor
{
public:
    MineSweeperScene();
public:
    void initial() override;
    void visit(uint32_t x, uint32_t y, std::shared_ptr<Tile> tile) override;
    void removeMine(size_t tileID);
    std::shared_ptr<TileMap> getTileMap()const;
private:
    void addMine(uint32_t x, uint32_t y, std::shared_ptr<Tile> tile);

    std::shared_ptr<MovingSprite> createSprite(const std::string &image, float x, float y);

    void onUpdateMyScene(float deltaTime) override;
private:
    bool shouldRemovalMine();
    bool shouldForward();
private:
    std::shared_ptr<TileMap> tileMap;
    std::map<size_t, SpritePointer> mines;
    std::shared_ptr<MovingSprite> mineSweeper;

    std::shared_ptr<State> errorState;
    std::shared_ptr<State> animationState;
    std::shared_ptr<State> forwardState;
    std::shared_ptr<State> turnLeftState;
    std::shared_ptr<State> turnRightState;
    std::shared_ptr<StateMachine> stateMachine;

    sf::Vector2i currentTile;
    std::optional<sf::Vector2i> targetTile;
    std::optional<float> targetRotate;
};
