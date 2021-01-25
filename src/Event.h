#ifndef __EVENT_H_
#define __EVENT_H_

#include <cstdint>

class Event
{
public:
    bool handled = false;
};

class WindowResizeEvent: public Event
{
private:
    int width, height;
public:
    WindowResizeEvent(int width, int height) : width(width), height(height) {}
    int getWidth() { return width; }
    int getHeight() { return height; }
};

class WindowCloseEvent: public Event
{
public:
    WindowCloseEvent() {}
};

class KeyEvent: public Event
{
public:
    enum Action {
        PRESS, RELEASE, REPEAT
    };
    KeyEvent(int key, int scancode, int mods, Action action)
        : key(key), scancode(scancode), mods(mods), action(action) {}
    int getKey() { return key; }
    int getScancode() { return scancode; }
    int getMods() { return mods; }
    Action getAction() { return action; }
private:
    int key;
    int scancode;
    int mods;
    Action action;
};

class CharEvent: public Event
{
public:
    CharEvent(uint32_t keycode) : keycode(keycode) {}
    uint32_t getKeycode() { return keycode; }

private:
    uint32_t keycode;
};

class MouseButtonEvent: public Event
{
public:
    enum Action {
        PRESS, RELEASE
    };
    MouseButtonEvent(int button, Action action, int mods) : button(button), action(action), mods(mods) {}
    int getButton() { return button; }
    Action getAction() { return action; }
    int getMods() { return mods; }

private:
    int button;
    Action action;
    int mods;
};

class MouseScrolledEvent: public Event
{
public:
    MouseScrolledEvent(double xOffset, double yOffset) : xOffset(xOffset), yOffset(yOffset) {}
    double getXOffset() { return xOffset; }
    double getYOffset() { return yOffset; }

private:
    double xOffset;
    double yOffset;
};

class MouseMovedEvent: public Event
{
public:
    MouseMovedEvent(double xPos, double yPos) : xPos(xPos), yPos(yPos) {}
    double getXPos() { return xPos; }
    double getYPos() { return yPos; }

private:
    double xPos;
    double yPos;
};

#endif // __EVENT_H_
