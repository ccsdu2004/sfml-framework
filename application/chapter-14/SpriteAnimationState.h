#pragma once
#include <Animation.h>
#include <StateMachine.h>

class MineSweeperScene;

class SpriteAnimationState : public State
{
public:
    SpriteAnimationState(std::shared_ptr<MineSweeperScene> inputScene, ObjectPointer inputSprite):
        State(inputSprite),
        scene(inputScene)
    {
        animation = createAnimation();
        auto fn = std::bind(&SpriteAnimationState::onFinished, this);
        animation->finished.connect(fn);
    }
public:
    void onEnter();
    void onLeave();
private:
    void onFinished()
    {
        setFinished();
    }

    AnimationPointer createAnimation();
private:
    std::shared_ptr<Animation> animation;
    std::weak_ptr<MineSweeperScene> scene;
    uint32_t tileID;
};
