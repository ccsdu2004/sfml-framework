#include <boost/signals2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Application.h>
#include <Entity.h>
#include <TileMap.h>
#include <Camera.h>
#include <MovingSprite.h>
#include <QuadTreeScene.h>
#include <StateMachine.h>
#include <Util.h>
#include "SpriteErrorState.h"
#include "SpriteAnimationState.h"
#include "SpriteForwardState.h"
#include "SpriteRotateState.h"
#include "WayFindingScene.h"

using namespace std;

int main()
{
    auto size = sf::Vector2f(1200, 860);
    auto setting = sf::ContextSettings();
    setting.antialiasingLevel = 12;
    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(size.x, size.y), "Chapter-35",
                  sf::Style::Close, setting);
    window->setVerticalSyncEnabled(true);

    auto app = Application::getInstance();
    app->setWindow(window);
    auto camera = std::make_shared<Camera>();
    app->addComponent(camera);
    camera->setWindowArea(sf::FloatRect(0, 0, size.x, size.y));

    auto scene = std::make_shared<WayFindingScene>();
    scene->initial();

    auto sceneManager = std::make_shared<SceneManager>();
    sceneManager->addScene(scene);
    sceneManager->setInitialScene(scene);

    app->execute(sceneManager);
    return 0;
}


