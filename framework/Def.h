#pragma once
#include <memory>

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

