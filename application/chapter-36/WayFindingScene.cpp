#include <sstream>
#include <Application.h>
#include <Camera.h>
#include <CameraTrackingObject.h>
#include <Fps.h>
#include <Widget/Desktop.h>
#include <Widget/Label.h>
#include <Widget/WidgetStyle.h>
#include "WayFindingScene.h"
#include "SpriteErrorState.h"
#include "SpriteIdleState.h"
#include "SpriteAnimationState.h"
#include "SpriteForwardState.h"
#include "SpriteRotateState.h"
#include "WayFindingTank.h"

WayFindingScene::WayFindingScene()
{
    tileMap = TileMap::createTileMap(TileMapType_Hex);
    tileMap->init(48, 31, 72);
    tileMap->setTextVisible(true);

    addChild(tileMap);
}

void WayFindingScene::initial()
{
    auto position = tileMap->getTileByIndex(0, 0)->getPosition();
    tank = std::make_shared<WayFindingTank>("../resource/images/tank/tank5.png", position.x,
                                            position.y);
    addChild(tank);

    tank->init(tileMap);
    Application::getInstance()->getComponent<Camera>()->setTarget(tank);

    shortPathStateMachine = std::make_shared<StateMachine>();

    toast = createToastText();
    toast->setSize(130, 36);
    toast->setText(L"寻路坦克", false);

    auto cameraTracking = std::make_shared<CameraTrackingObject>();
    cameraTracking->setTarget(toast);
    cameraTracking->setTargetPosition(sf::Vector2f(20, 20));

    addChild(cameraTracking);

    wayFindingTileVisitor = std::make_shared<WayFindingTileVisitor>(tileMap);
    wayFindingTileVisitor->compute(tank, sf::Vector2i(0, 0));

    wayFindingState = std::make_shared<WayFindingState>(tileMap, tank, wayFindingTileVisitor);
    shortPathStateMachine->addState(wayFindingState);
    shortPathStateMachine->setInitState(wayFindingState);
    shortPathStateMachine->setErrorState(std::make_shared<WayFindingErrorState>(tileMap, tank));
    shortPathStateMachine->start();
}

void WayFindingScene::onUpdateMyScene(float deltaTime)
{
    shortPathStateMachine->update(deltaTime);
    float fps = Application::getInstance()->getComponent<Fps>()->getFps();

    std::wstringstream stream;
    stream.precision(4);
    stream << L"寻路坦克 ";
    stream << "FPS:";
    stream << fps;
    toast->setText(stream.str(), false);
}






