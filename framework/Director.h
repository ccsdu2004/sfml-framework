#pragma once
#include <Component.h>
#include <Scene.h>

class Director : public Component
{
public:
    Director();
    virtual ~Director();
public:
    void setScene(ScenePointer scene);
    ScenePointer getScene()const;

    void detachSpriteFromScene(SpritePointer object);
private:
    std::unique_ptr<class DirectorData> data;
};


