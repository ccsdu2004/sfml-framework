#include "Application.h"
#include "Widget/Desktop.h"
#include "Widget/Label.h"
#include "Widget/WidgetStyle.h"
#include "WayFindingScene.h"
#include "SpriteErrorState.h"
#include "SpriteIdleState.h"
#include "SpriteAnimationState.h"
#include "SpriteForwardState.h"
#include "SpriteRotateState.h"
#include "WayFindingTank.h"

class HelpListener : public MessageListener
{
    // MessageListener interface
public:
    HelpListener()
    {
        helpLabel = std::make_shared<Label>();
        helpLabel->setSize(320, 132);
        helpLabel->setText(L"自动寻路坦克\n随机设置目标地址和路障\n计算最短路径\n坦克通过转向和前进到达目的地\n然后重新设置目的地并以此反复");

        auto style = std::make_shared<LabelStyle>();
        style->textColor = sf::Color::White;
        style->size = 21.0f;
        style->vMode = VMode_Top;
        helpLabel->setWidgetStyle(style);

        auto desktop = Application::getInstance()->getComponent<Desktop>();
        desktop->addWidget(helpLabel, HMode_Right, VMode_Top);
    }

    bool onListener(std::shared_ptr<Message> message) override
    {
        if(message->getType() == Message_SFML) {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {
                helpLabel->setVisible(!helpLabel->isVisible());
                return true;
            }
        }
        return false;
    }
private:
    std::shared_ptr<Label> helpLabel;
};

MineSweeperScene::MineSweeperScene()
{
    tileMap = TileMap::createTileMap(TileMapType_Hex);
    tileMap->init(23, 15, 36);
    tileMap->setTextVisible(true);

    addChild(tileMap);
}

void MineSweeperScene::initial()
{
    auto position = tileMap->getTileByIndex(0, 0)->getPosition();
    tank = std::make_shared<WayFindingTank>("../resource/images/tank/tank5.png", position.x,
                                            position.y);
    addChild(tank);

    tank->init(tileMap);

    shortPathStateMachine = std::make_shared<StateMachine>();

    auto text = createToastText();
    text->setBackgroundColor(sf::Color(0, 0, 0, 160));
    text->setText(L"寻路坦克", false);
    addChild(text);

    wayFindingTileVisitor = std::make_shared<WayFindingTileVisitor>(tileMap);
    wayFindingTileVisitor->compute(sf::Vector2i(0, 0));

    auto helpListener = std::make_shared<HelpListener>();
    addMessageListener(helpListener);

    wayFindingState = std::make_shared<WayFindingState>(tileMap, tank, wayFindingTileVisitor);
    shortPathStateMachine->addState(wayFindingState);
    shortPathStateMachine->setInitState(wayFindingState);
    shortPathStateMachine->setErrorState(std::make_shared<WayFindingErrorState>(tileMap, tank));
    shortPathStateMachine->start();
}

void MineSweeperScene::onUpdateMyScene(float deltaTime)
{
    shortPathStateMachine->update(deltaTime);
}






