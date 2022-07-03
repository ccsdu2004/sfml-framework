#include <cassert>
#include <ctime>
#include <SFML/System/Sleep.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <Application.h>
#include <Message.h>
#include <Scene.h>
#include <ResourceManager.h>
#include <Widget/Desktop.h>

class ApplicationData
{
public:
    std::shared_ptr<sf::RenderWindow> window;
    std::shared_ptr<Object> rootObject;
    sf::Color backgroundColor;

    std::map<std::string, std::shared_ptr<sf::Font>> fonts;
    std::map<std::string, std::shared_ptr<sf::Image>> images;
    std::map<std::string, std::shared_ptr<sf::Texture>> textures;

    std::shared_ptr<Desktop> desktop;
};

std::shared_ptr<Application> Application::instance(new Application());

std::shared_ptr<Application> Application::getInstance()
{
    return instance;
}

Application::~Application()
{
}

void Application::setBackgroundColor(const sf::Color &color)
{
    data->backgroundColor = color;
}

bool Application::execute(std::shared_ptr<Object> object)
{
    assert(data->window);
    assert(object);

    data->rootObject = object;

    auto fn = [&]()->std::shared_ptr<MessageReceiver> {return object;};
    auto listener = std::make_shared<ProxyMessageListener<Object>>(fn);
    addMessageListener(listener);

    sf::Clock clock;
    while (data->window->isOpen()) {
        sf::Event event;
        while (data->window->pollEvent(event)) {
            if ((event.type == sf::Event::Closed) ||
                ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))) {
                data->window->close();
                break;
            }

            auto message = std::make_shared<SFMLMessage>(event);
            object->process(message);
        }

        data->window->clear(data->backgroundColor);
        data->window->setActive(true);

        float deltaTime = clock.restart().asSeconds();
        object->update(deltaTime);
        updateComponent(deltaTime);

        data->window->draw(*object);
        data->window->draw(*data->desktop);

        /*sf::View view;
        view.setViewport(sf::FloatRect(0, 0, 0.5, 0.5));
        data->window->setView(view);
        data->window->draw(*object);

        view = data->window->getDefaultView();
        data->window->setView(view);*/

        data->window->display();

        sf::sleep(sf::milliseconds(10));
    }

    return true;
}

void Application::setWindow(std::shared_ptr<sf::RenderWindow> inputWindow)
{
    data->window = inputWindow;
}

std::shared_ptr<sf::RenderWindow> Application::getWindow() const
{
    return data->window;
}

std::shared_ptr<SceneManager> Application::getSceneManager() const
{
    return std::dynamic_pointer_cast<SceneManager>(data->rootObject);
}

void Application::exit()
{
    data->window->close();
}

std::shared_ptr<sf::Texture> Application::loadTexture(const std::string& file, const sf::IntRect& area)
{
    auto itr = data->textures.find(file);
    if (itr != data->textures.end())
        return (*itr).second;

    auto texture = std::make_shared<sf::Texture>();
    texture->loadFromFile(file, area);
    data->textures.insert(std::make_pair(file, texture));
    return texture;
}

Application::Application():
    data(new ApplicationData())
{
    srand(time(nullptr));

    data->desktop = std::make_shared<Desktop>();
    addComponent(data->desktop);

    auto imageResourceManager = std::make_shared<ResourceManager<sf::Image>>();
    addComponent(imageResourceManager);

    auto textureResourceManager = std::make_shared<ResourceManager<sf::Texture>>();
    addComponent(textureResourceManager);

    auto fontResourceManager = std::make_shared<ResourceManager<sf::Font>>();
    addComponent(fontResourceManager);
}
