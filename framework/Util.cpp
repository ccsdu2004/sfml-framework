#include <Util.h>

sf::FloatRect expandRect(const sf::FloatRect& a, const sf::FloatRect& b)
{
    sf::FloatRect output(a);
    if(b.left < output.left)
        output.left = b.left;
    if(b.top < output.top)
        output.top = b.top;

    if(output.left + output.width < b.left + b.width)
        output.width = b.left + b.width - output.left;
    if(output.top + output.height < b.top + b.height)
        output.height = b.top + b.height - output.top;
    return output;
}
