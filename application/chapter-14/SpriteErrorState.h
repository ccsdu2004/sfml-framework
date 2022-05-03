#pragma once
#include <iostream>
#include <StateMachine.h>

class SpriteErrorState : public State
{
public:
    SpriteErrorState(ObjectPointer inputSprite):
        State(inputSprite)
    {
    }
public:
    void enter()
    {
        std::cout << "enter SpriteErrorState" << std::endl;
    }

    void leave()
    {
        std::cout << "leave SpriteErrorState" << std::endl;
    }
private:
};
