#ifndef __EVENT_H_
#define __EVENT_H_

#include <cstdint>
#include <functional>

#define EVENT_TYPE(type)                                   \
    virtual Type getType() const override { return type; } \
    static Type staticType() { return type; }

class Event {
public:
    enum Type {
        None,
        WindowResize,
        WindowClose,
        Key,
        Char,
        MouseButton,
        MouseMoved,
        MouseScrolled,
    };
    bool handled = false;

    virtual Type getType() const { return None; }
};

class WindowResizeEvent : public Event {
private:
    int width, height;

public:
    WindowResizeEvent(int width, int height)
        : width(width)
        , height(height)
    {
    }
    int getWidth() { return width; }
    int getHeight() { return height; }

    EVENT_TYPE(WindowResize)
};

class WindowCloseEvent : public Event {
public:
    WindowCloseEvent() { }
    EVENT_TYPE(WindowClose)
};

class KeyEvent : public Event {
public:
    enum Action {
        PRESS,
        RELEASE,
        REPEAT
    };
    KeyEvent(int key, int scancode, int mods, Action action)
        : key(key)
        , scancode(scancode)
        , mods(mods)
        , action(action)
    {
    }
    int getKey() { return key; }
    int getScancode() { return scancode; }
    int getMods() { return mods; }
    Action getAction() { return action; }

    EVENT_TYPE(Key)
private:
    int key;
    int scancode;
    int mods;
    Action action;
};

class CharEvent : public Event {
public:
    CharEvent(uint32_t keycode)
        : keycode(keycode)
    {
    }
    uint32_t getKeycode() { return keycode; }

    EVENT_TYPE(Char)

private:
    uint32_t keycode;
};

class MouseButtonEvent : public Event {
public:
    enum Action {
        PRESS,
        RELEASE
    };
    MouseButtonEvent(int button, Action action, int mods)
        : button(button)
        , action(action)
        , mods(mods)
    {
    }
    int getButton() { return button; }
    Action getAction() { return action; }
    int getMods() { return mods; }

    EVENT_TYPE(MouseButton)
private:
    int button;
    Action action;
    int mods;
};

class MouseScrolledEvent : public Event {
public:
    MouseScrolledEvent(double xOffset, double yOffset)
        : xOffset(xOffset)
        , yOffset(yOffset)
    {
    }
    double getXOffset() { return xOffset; }
    double getYOffset() { return yOffset; }

    EVENT_TYPE(MouseScrolled)
private:
    double xOffset;
    double yOffset;
};

class MouseMovedEvent : public Event {
public:
    MouseMovedEvent(double xPos, double yPos)
        : xPos(xPos)
        , yPos(yPos)
    {
    }
    double getXPos() { return xPos; }
    double getYPos() { return yPos; }

    EVENT_TYPE(MouseMoved)
private:
    double xPos;
    double yPos;
};

template <typename T, typename F>
bool DispatchEvent(Event& event, const F& func)
{
    if (event.getType() == T::staticType()) {
        event.handled = func(static_cast<T&>(event));
        return true;
    }
    return false;
}

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

#endif // __EVENT_H_
