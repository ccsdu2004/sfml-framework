#pragma once
#include <memory>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Object.h>
#include <Message.h>
#include <Component.h>

class Application : public MessageReceiver, public ComponentPool
{
    static std::shared_ptr<Application> instance;
public:
    static std::shared_ptr<Application> getInstance();
    virtual ~Application();
public:
    void setBackgroundColor(const sf::Color &color);
    bool execute(std::shared_ptr<Object> object);

    void setWindow(std::shared_ptr<sf::RenderWindow> window);
    std::shared_ptr<sf::RenderWindow> getWindow()const;

    std::shared_ptr<SceneManager> getSceneManager()const;

    void exit();
public:
    std::shared_ptr<sf::Texture> loadTexture(const std::string& file, const sf::IntRect& area = sf::IntRect());
private:
    Application();
    std::unique_ptr<class ApplicationData> data;
};
