#ifndef WINDOWOBJECT_H
#define WINDOWOBJECT_H
#include <SFML/Graphics/RenderWindow.hpp>
#include <Object.h>

class WindowObject : public Object
{
public:
    WindowObject();
public:
    void setWindow(std::shared_ptr<sf::RenderWindow> window);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
private:
    std::shared_ptr<sf::RenderWindow> window;
};

#endif // WINDOWOBJECT_H
