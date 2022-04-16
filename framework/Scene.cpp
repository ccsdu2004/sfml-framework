#include <iostream>
#include <optional>
#include <unordered_map>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <Scene.h>

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
    std::shared_ptr<Scene> currentScene;
    std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;
};

SceneManager::SceneManager():
    data(new SceneManagerImpl())
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::setInitialScene(std::shared_ptr<Scene> scene)
{
    if (!scene)
        return;
    data->currentScene = scene;
    data->scenes.insert(std::make_pair(scene->getName(), scene));
}

void SceneManager::addScene(std::shared_ptr<Scene> scene)
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
    return false;
}

void SceneManager::update(const sf::Time &time)
{
    if (data->currentScene) {
        data->currentScene->update(time);
    }
}

void SceneManager::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (data->currentScene)
        data->currentScene->draw(target, states);
}
