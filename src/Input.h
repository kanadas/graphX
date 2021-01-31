#ifndef __INPUT_H_
#define __INPUT_H_

#include "Event.h"
#include <unordered_set>

class Input {
public:
    Input(const Input&) = delete;

    static void init(float windowWidth, float windowHeight);

    inline static bool IsKeyPressed(int keycode) { return instance->isKeyPressedImpl(keycode); }

    inline static bool IsMouseButtonPressed(int button) { return instance->isMouseButtonPressedImpl(button); }
    inline static std::pair<float, float> GetMousePosition() { return instance->getMousePositionImpl(); }
    inline static float GetMouseX() { return instance->getMouseXImpl(); }
    inline static float GetMouseY() { return instance->getMouseYImpl(); }
    inline static float GetWindowWidth() { return instance->getWindowWidthImpl(); }
    inline static float GetWindowHeight() { return instance->getWindowHeightImpl(); }
    inline static bool IsImGuiWindowHovered() { return instance->ImGuiWindowHovered; }

    static void onEvent(Event& e) { instance->onEventImpl(e); }
    static void onImGui() { instance->onImGuiImpl(); }
private:
    static Input* instance;
    Input(float windowWidth, float windowHeight);

    std::unordered_set<int> pressedKeys;
    std::unordered_set<int> pressedMouseButtons;
    bool isKeyPressedImpl(int keycode);
    bool isMouseButtonPressedImpl(int button);

    float mousePosX = 0.0f;
    float mousePosY = 0.0f;
    std::pair<float, float> getMousePositionImpl() { return std::make_pair(mousePosX, mousePosY); };
    float getMouseXImpl() { return mousePosX; };
    float getMouseYImpl() { return mousePosY; };
    float windowWidth;
    float windowHeight;
    float getWindowWidthImpl() { return windowWidth; }
    float getWindowHeightImpl() { return windowHeight; }

    void onEventImpl(Event& e);
    bool onKey(KeyEvent& e);
    bool onMouseButton(MouseButtonEvent& e);
    bool onMouseMoved(MouseMovedEvent& e);
    bool onWindowResize(WindowResizeEvent& e);

    bool ImGuiWindowHovered = false;
    void onImGuiImpl();
};

#endif // __INPUT_H_
