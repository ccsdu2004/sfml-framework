#pragma once
#include <Animation.h>
#include <StateMachine.h>

class WayFindingScene;

class SpriteAnimationState : public State
{
public:
    SpriteAnimationState(std::shared_ptr<WayFindingScene> inputScene, ObjectPointer inputSprite):
        State(inputSprite),
        scene(inputScene)
    {
        animation = createAnimation();
        auto fn = std::bind(&SpriteAnimationState::onFinished, this);
        animation->finished.connect(fn);
    }
public:
    void onEnter() override;
    void onLeave() override;
    void update(float deltaTime) override;
private:
    void onFinished()
    {
        setFinished();
    }

    AnimationPointer createAnimation();
private:
    std::shared_ptr<Animation> animation;
    std::weak_ptr<WayFindingScene> scene;
    uint32_t tileID;
};
