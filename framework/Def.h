#pragma once
#include <memory>
#include <list>

enum SpriteGroupID : uint32_t {
    SpriteGroupID_Bullet = 0,
    SpriteGroupID_PlayerA,
    SpriteGroupID_PlayerB,
    SpriteGroupID_PlayerC,
    SpriteGroupID_PlayerD,
    SpriteGroupID_Max
};

class Object;
using ObjectPointer = std::shared_ptr<Object>;

class Entity;
using EntityPointer = std::shared_ptr<Entity>;

class Sprite;
using SpritePointer = std::shared_ptr<Sprite>;

class SpriteGroup;
using SpriteGroupPointer = std::shared_ptr<SpriteGroup>;

class Switch;
using SwitchPointer = std::shared_ptr<Switch>;

class Animation;
using AnimationPointer = std::shared_ptr<Animation>;

class Scene;
using ScenePointer = std::shared_ptr<Scene>;
using SceneWeakPointer = std::weak_ptr<Scene>;

class SpriteDeleter;
using SpriteDeleterPointer = std::shared_ptr<SpriteDeleter>;

class SpriteOperator;
using SpriteOperatorPointer = std::shared_ptr<SpriteOperator>;
