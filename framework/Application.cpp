#include <cassert>
#include <ctime>
#include <SFML/System/Sleep.hpp>
#include <SFML/Window/Event.hpp>
#include <Application.h>
#include <Message.h>
#include <Widget/Desktop.h>
#include <SFML/Graphics/CircleShape.hpp>

class ApplicationData
{
public:
    std::shared_ptr<sf::RenderWindow> window;
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

void Application::exit()
{
    data->window->close();
}

std::shared_ptr<sf::Font> Application::loadFont(const std::string file)
{
    auto itr = data->fonts.find(file);
    if (itr != data->fonts.end())
        return (*itr).second;

    auto font = std::make_shared<sf::Font>();
    font->loadFromFile(file);
    data->fonts.insert(std::make_pair(file, font));
    return font;
}

std::shared_ptr<sf::Image> Application::loadImage(const std::string file)
{
    auto itr = data->images.find(file);
    if (itr != data->images.end())
        return (*itr).second;

    auto image = std::make_shared<sf::Image>();
    image->loadFromFile(file);
    data->images.insert(std::make_pair(file, image));
    return image;
}

std::shared_ptr<sf::Texture> Application::loadTexture(const std::string file)
{
    auto itr = data->textures.find(file);
    if (itr != data->textures.end())
        return (*itr).second;

    auto texture = std::make_shared<sf::Texture>();
    texture->loadFromFile(file);
    data->textures.insert(std::make_pair(file, texture));
    return texture;
}

Application::Application():
    data(new ApplicationData())
{
    srand(time(nullptr));

    data->desktop = std::make_shared<Desktop>();
    addComponent(data->desktop);
}
