#include <map>
#include <optional>
#include <unordered_map>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <Scene.h>
#include <Widget/Desktop.h>
#include <Application.h>

class SceneData
{
public:
    SceneData()
    {
        backgrpundSprite.setPosition(0, 0);
    }
public:
    std::optional<sf::Texture> background;
    sf::Sprite backgrpundSprite;
};

Scene::Scene():
    data(new SceneData())
{
}

Scene::~Scene()
{
}

sf::FloatRect Scene::getBoundingBox() const
{
    auto size = Application::getInstance()->getWindow()->getSize();
    return sf::FloatRect(0, 0, size.x, size.y);
}

void Scene::setBackground(const sf::Texture &texture)
{
    data->background = texture;
    data->backgrpundSprite.setTexture(texture);
}

std::shared_ptr<Text> Scene::createToastText()
{
    auto font = Application::getInstance()->loadFont("../resource/FZYTK.TTF");

    auto text = std::make_shared<Text>();
    text->setFont(font);
    text->setCharacterSize(15);
    text->setTextColor(sf::Color::White);
    text->setSize(120, 36);
    text->setBackgroundColor(sf::Color::Black);

    text->setOutlineColor(sf::Color::Yellow);
    text->setOutlineThickness(0.6f);

    text->setPosition(80.0f, 30.0f);
    return text;
}

void Scene::initial()
{

}

void Scene::enter()
{
}

void Scene::exit()
{
}

void Scene::release()
{
}

bool Scene::process(std::shared_ptr<Message> message)
{
    bool done = false;
    auto desktop = getComponent<Desktop>();
    if (desktop)
        done = desktop->process(message);
    return done ? true : Object::process(message);
}

void Scene::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(data->backgrpundSprite, states);
    Object::draw(target, states);

    auto desktop = getComponent<Desktop>();
    if (desktop)
        desktop->draw(target, states);
}

void Scene::onUpdateObject(float deltaTime)
{
    auto desktop = getComponent<Desktop>();
    if (desktop)
        desktop->update(deltaTime);
}

class SceneManagerImpl
{
public:
    ScenePointer currentScene;
    std::unordered_map<std::string, ScenePointer> scenes;
};

SceneManager::SceneManager():
    data(new SceneManagerImpl())
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::setInitialScene(ScenePointer scene)
{
    if (!scene)
        return;
    data->currentScene = scene;
    data->scenes.insert(std::make_pair(scene->getName(), scene));
}

void SceneManager::addScene(ScenePointer scene)
{
    if (!scene)
        return;

    auto name = scene->getName();
    if (data->scenes.find(name) != data->scenes.end()) {
        std::cerr << "scene name:" << name << " already exists!";
        return;
    }
    data->scenes.insert(std::make_pair(scene->getName(), scene));
}

ScenePointer SceneManager::getCurrentScene() const
{
    return data->currentScene;
}

void SceneManager::switchTo(const std::string &scene)
{
    auto find = data->scenes.find(scene);
    if (find == data->scenes.end())
        return;

    if (data->currentScene && data->currentScene->getName() == scene)
        return;

    if (data->currentScene)
        data->currentScene->exit();

    data->currentScene = find->second;;
    data->currentScene->enter();
}

bool SceneManager::process(std::shared_ptr<Message> message)
{
    bool done = false;
    if (data->currentScene)
        done = data->currentScene->process(message);
    if(!done)
        return Object::process(message);
    return done;
}

void SceneManager::onUpdateObject(float deltaTime)
{
    if (data->currentScene) {
        data->currentScene->update(deltaTime);
    }

    Object::onUpdateObject(deltaTime);
}

void SceneManager::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (data->currentScene)
        data->currentScene->draw(target, states);
    Object::draw(target, states);
}
