#include <iostream>
#include <map>
#include <optional>
#include <unordered_map>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <Scene.h>
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

void Scene::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(data->backgrpundSprite, states);
    Object::draw(target, states);
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
    if (data->currentScene)
        return data->currentScene->process(message);
    return Object::process(message);
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
