#include "Input.h"

#include "utils/Log.h"
#include "Application.h"

Input* Input::instance = nullptr;

Input::Input(float windowWidth, float windowHeight)
    : windowWidth(windowWidth)
    , windowHeight(windowHeight)
{
}

void Input::init(float windowWidth, float windowHeight)
{
    if (instance == nullptr) {
        TRACE("Initializing input");
        instance = new Input(windowWidth, windowHeight);
    }
}

void Input::onEventImpl(Event& e)
{
    DispatchEvent<KeyEvent>(e, BIND_EVENT_FN(Input::onKey));
    DispatchEvent<MouseButtonEvent>(e, BIND_EVENT_FN(Input::onMouseButton));
    DispatchEvent<MouseMovedEvent>(e, BIND_EVENT_FN(Input::onMouseMoved));
    DispatchEvent<WindowResizeEvent>(e, BIND_EVENT_FN(Input::onWindowResize));
}

bool Input::onKey(KeyEvent& e)
{
    if (e.getAction() == KeyEvent::Action::PRESS) {
        TRACE("Pressed key {}", e.getKey());
        pressedKeys.insert(e.getKey());
    } else if (e.getAction() == KeyEvent::Action::RELEASE) {
        TRACE("Released key {}", e.getKey());
        pressedKeys.erase(e.getKey());
    }
    return false;
}

bool Input::onMouseButton(MouseButtonEvent& e)
{
    if (e.getAction() == MouseButtonEvent::Action::PRESS) {
        TRACE("Pressed mouse {}", e.getButton());
        pressedMouseButtons.insert(e.getButton());
    } else if (e.getAction() == MouseButtonEvent::Action::RELEASE) {
        TRACE("Released mouse {}", e.getButton());
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

bool Input::onWindowResize(WindowResizeEvent& e)
{
    windowHeight = e.getHeight();
    windowWidth = e.getWidth();
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
