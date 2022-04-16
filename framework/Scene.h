#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <Object.h>
#include <NameHolder.h>
#include <Component.h>

class Scene : public Object, public NameHolder, public ComponentHolder
{
public:
    Scene();
    virtual ~Scene();
public:
    void setBackground(const sf::Texture& texture);
public:
    virtual void initial();
    virtual void enter();
    virtual void exit();
    virtual void release();
public:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
    std::unique_ptr<class SceneData> data;
};

class SceneManager : public Object
{
public:
    SceneManager();
    ~SceneManager();
public:
    void setInitialScene(std::shared_ptr<Scene> scene);
    void addScene(std::shared_ptr<Scene> scene);
public:
    void switchTo(const std::string &scene);
public:
    bool process(std::shared_ptr<Message> message)override;
    void update(const sf::Time &time) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
private:
    std::unique_ptr<class SceneManagerImpl> data;
};
