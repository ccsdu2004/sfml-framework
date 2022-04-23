#include <Director.h>
#include <Sprite.h>

class DirectorData
{
public:
    ScenePointer scene;
};

Director::Director():
    data(new DirectorData)
{
}

Director::~Director()
{
}

void Director::setScene(ScenePointer scene)
{
    data->scene = scene;
}

ScenePointer Director::getScene() const
{
    return data->scene;
}

void Director::detachSpriteFromScene(SpritePointer object)
{
    if(!object)
        return;

    if(SpriteStatus_Death == object->getSpriteStatus() && data->scene)
        data->scene->removeChild(object);
}
