#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <Object.h>
#include <NameHolder.h>
#include <Component.h>
#include <Sprite.h>
#include <Text.h>

class Scene : public Object, public ComponentPool, public NameHolder
{
public:
    Scene();
    virtual ~Scene();
public:
    sf::FloatRect getBoundingBox()const;
    void setBackground(const sf::Texture &texture);

    std::shared_ptr<Text> createToastText();
public:
    virtual void initial();
    virtual void enter();
    virtual void exit();
    virtual void release();
public:
    virtual bool process(std::shared_ptr<Message> message)override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    virtual void onUpdateObject(float deltaTime) override;
private:
    std::unique_ptr<class SceneData> data;
};

using ScenePointer = std::shared_ptr<Scene>;

class SceneManager : public Object
{
public:
    SceneManager();
    ~SceneManager();
public:
    void setInitialScene(ScenePointer scene);
    void addScene(ScenePointer scene);
public:
    void switchTo(const std::string &scene);
public:
    bool process(std::shared_ptr<Message> message)override;
    void onUpdateObject(float deltaTime) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
private:
    std::unique_ptr<class SceneManagerImpl> data;
};
