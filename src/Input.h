#ifndef __INPUT_H_
#define __INPUT_H_

#include "Event.h"
#include <unordered_set>

class Input {
public:
    Input(const Input&) = delete;

    static void init();

    inline static bool isKeyPressed(int keycode) { return instance->isKeyPressedImpl(keycode); }

    inline static bool isMouseButtonPressed(int button) { return instance->isMouseButtonPressedImpl(button); }
    inline static std::pair<float, float> GetMousePosition() { return instance->getMousePositionImpl(); }
    inline static float GetMouseX() { return instance->getMouseXImpl(); }
    inline static float GetMouseY() { return instance->getMouseYImpl(); }

    static void onEvent(Event& e) { instance->onEventImpl(e); };
private:
    static Input* instance;
    Input();

    std::unordered_set<int> pressedKeys;
    std::unordered_set<int> pressedMouseButtons;
    bool isKeyPressedImpl(int keycode);
    bool isMouseButtonPressedImpl(int button);

    float mousePosX = 0.0f;
    float mousePosY = 0.0f;
    std::pair<float, float> getMousePositionImpl();
    float getMouseXImpl();
    float getMouseYImpl();

    void onEventImpl(Event& e);
    bool onKey(KeyEvent& e);
    bool onMouseButton(MouseButtonEvent& e);
    bool onMouseMoved(MouseMovedEvent& e);
};

#endif // __INPUT_H_
