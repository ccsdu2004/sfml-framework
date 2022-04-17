#pragma once
#include <memory>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <Object.h>
#include <Message.h>

class Application : public MessageReceiver
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
public:
    std::shared_ptr<sf::Font> loadFont(const std::string file);
    std::shared_ptr<sf::Image> loadImage(const std::string file);
    std::shared_ptr<sf::Texture> loadTexture(const std::string file);
private:
    Application();
    std::unique_ptr<class ApplicationData> data;
};
