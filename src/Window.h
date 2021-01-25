#ifndef __WINDOW_H_
#define __WINDOW_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <functional>
#include "Event.h"

class Window {
public:
    Window(const std::string& title, uint32_t width, uint32_t height, std::function<void(Event&)> callback);
    ~Window();

    void onUpdate();
    void setVSync(bool enable);

    inline uint32_t getWidth() { return windowData.width; }
    inline uint32_t getHeight() { return windowData.height; }
    inline bool isVSync() { return windowData.VSync; }
    inline void setEventCallback(std::function<void(Event&)> callback) { windowData.eventCallback = callback; }
    inline GLFWwindow* getNativeWindow() { return window; }

private:
    GLFWwindow* window;

    struct WindowData {
        uint32_t width, height;
        bool VSync;
        std::function<void(Event&)> eventCallback;
    };

    WindowData windowData;

    void shutdown();
};

#endif // __WINDOW_H_
