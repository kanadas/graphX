#include "Input.h"

Input* Input::instance = nullptr;

Input::Input()
{

}

void Input::init()
{
    if(instance == nullptr) {
        instance = new Input();
    }
}

void Input::onEventImpl(Event& e)
{
    DispatchEvent<KeyEvent>(e, BIND_EVENT_FN(Input::onKey));
    DispatchEvent<MouseButtonEvent>(e, BIND_EVENT_FN(Input::onMouseButton));
    DispatchEvent<MouseMovedEvent>(e, BIND_EVENT_FN(Input::onMouseMoved));
}


bool Input::onKey(KeyEvent& e)
{
    if(e.getAction() == KeyEvent::Action::PRESS) {
        pressedKeys.insert(e.getKey());
    } else if(e.getAction() == KeyEvent::Action::RELEASE) {
        pressedKeys.erase(e.getKey());
    }
    return false;
}

bool Input::onMouseButton(MouseButtonEvent& e)
{
    if(e.getAction() == MouseButtonEvent::Action::PRESS) {
        pressedMouseButtons.insert(e.getButton());
    } else if(e.getAction() == MouseButtonEvent::Action::RELEASE) {
        pressedMouseButtons.erase(e.getButton());
    }
    return false;
}

bool Input::onMouseMoved(MouseMovedEvent& e)
{
    mousePosX = e.getXPos();
    mousePosY = e.getXPos();
    return false;
}

bool Input::isKeyPressedImpl(int keycode)
{
    return pressedKeys.count(keycode) > 0;
}

bool Input::isMouseButtonPressedImpl(int button)
{
    return pressedMouseButtons.count(button) > 0;
}

std::pair<float, float> Input::getMousePositionImpl()
{
    return std::make_pair(mousePosX, mousePosY);
}

float Input::getMouseXImpl()
{
    return mousePosX;
}

float Input::getMouseYImpl()
{
    return mousePosY;
}
