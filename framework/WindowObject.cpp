#include <cassert>
#include "WindowObject.h"

WindowObject::WindowObject()
{

}

void WindowObject::setWindow(std::shared_ptr<sf::RenderWindow> inputWindow)
{
    window = inputWindow;
}

void WindowObject::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    assert(window);
}
