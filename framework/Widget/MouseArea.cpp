#include <Widget/MouseArea.h>
#include <Widget/Label.h>
#include <Widget/Desktop.h>
#include <Scene.h>
#include <Application.h>

class MouseAreaData
{
public:
    bool movable = true;
    sf::Vector2i oldMousePosition;
    std::weak_ptr<Widget> target;

    std::shared_ptr<Label> label;
};

MouseArea::MouseArea(const sf::Vector2f &size, const std::optional<CornerStyle> &cornerStyle):
    Widget(size, cornerStyle),
    data(new MouseAreaData)
{
    data->label = std::make_shared<Label>();
    addChild(data->label);
    setObjectRenderOrder(ObjectRenderOrder_ParentFirst);
}

MouseArea::~MouseArea()
{
}

void MouseArea::setTargetWidget(std::shared_ptr<Widget> target)
{
    data->target = target;
}

std::shared_ptr<Widget> MouseArea::getTargetWidget() const
{
    return data->target.expired() ? nullptr : data->target.lock();
}

void MouseArea::setText(const sf::String &text)
{
    data->label->setText(text);
}

sf::String MouseArea::getText() const
{
    return data->label->getText();
}

void MouseArea::onMouseEnter()
{
    data->oldMousePosition = sf::Mouse::getPosition(
                                 *Application::getInstance()->getWindow());
}

void MouseArea::onMouseExit()
{
}

void MouseArea::onMousePressed(sf::Mouse::Button button)
{
    (void)button;
    data->oldMousePosition = sf::Mouse::getPosition(
                                 *Application::getInstance()->getWindow());

    auto sceneManager = Application::getInstance()->getSceneManager();
    if(sceneManager && sceneManager->getCurrentScene()) {
        auto desktop = sceneManager->getCurrentScene()->getComponent<Desktop>();
        desktop->setFocusWidget(std::dynamic_pointer_cast<Widget>(shared_from_this()));
    }
}

void MouseArea::onMouseReleased(sf::Mouse::Button button)
{
    (void)button;
    auto sceneManager = Application::getInstance()->getSceneManager();
    if(sceneManager && sceneManager->getCurrentScene()) {
        auto desktop = sceneManager->getCurrentScene()->getComponent<Desktop>();
        desktop->setFocusWidget(nullptr);
    }
}

void MouseArea::onMouseMoved(int x, int y)
{
    if (data->target.expired())
        return;

    auto widget = data->target.lock();

    if (data->oldMousePosition != sf::Vector2i(x, y) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        int dx = x - data->oldMousePosition.x;
        int dy = y - data->oldMousePosition.y;
        widget->move(dx, dy);
        data->oldMousePosition.x = x;
        data->oldMousePosition.y = y;

        moved();
    }
}

void MouseArea::onStyleChanged()
{
    Widget::onStyleChanged();
    auto style = std::dynamic_pointer_cast<LabelStyle>(getWidgetStyle());
    data->label->setWidgetStyle(style);
}

