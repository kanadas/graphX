#include "Window.h"

#include "utils/Log.h"
#include "utils/errors.h"
#include "utils/stacktrace.cxx"

static bool glfwInitialized = false;

static void GLFWErrorCallback(int error, const char* description)
{
    ERROR("Error {}: {}\n{}", error, description, Backtrace());
}

Window::Window(const std::string& title, uint32_t width, uint32_t height, std::function<void(Event&)> callback)
{
    TRACE("Creating window {} {} {}", title, width, height);
    windowData.width = width;
    windowData.height = height;
    windowData.eventCallback = callback;

    if (!glfwInitialized) {
        TRACE("Initializing GLFW");
        if (!glfwInit()) {
            ERROR("GLFW initialization failed!\n");
            exit(1);
        }
        glfwSetErrorCallback(GLFWErrorCallback);
        glfwInitialized = true;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!window) {
        ERROR("Window or OpenGL context creation failed!\n");
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        ERROR("Glad failed to load OpenGL!\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(1);
    }
    glfwSwapInterval(1);
    glfwSetWindowUserPointer(window, &windowData);
    setVSync(true);

    INFO("OpenGL {}, GLSL {}\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.width = width;
        data.height = height;
        WindowResizeEvent event(width, height);
        data.eventCallback(event);
    });

    glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        WindowCloseEvent event;
        data.eventCallback(event);
    });

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        KeyEvent::Action keyAction;
        switch (action) {
        case GLFW_PRESS: keyAction = KeyEvent::Action::PRESS; break;
        case GLFW_RELEASE: keyAction = KeyEvent::Action::RELEASE; break;
        case GLFW_REPEAT: keyAction = KeyEvent::Action::REPEAT; break;
        }
        KeyEvent event(key, scancode, mods, keyAction);
        data.eventCallback(event);
    });

    glfwSetCharCallback(window, [](GLFWwindow* window, uint32_t keycode) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        CharEvent event(keycode);
        data.eventCallback(event);
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseButtonEvent::Action keyAction;
        switch (action) {
        case GLFW_PRESS: keyAction = MouseButtonEvent::Action::PRESS; break;
        case GLFW_RELEASE: keyAction = MouseButtonEvent::Action::RELEASE; break;
        }
        MouseButtonEvent event(button, keyAction, mods);
        data.eventCallback(event);
    });

    glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseScrolledEvent event(xOffset, yOffset);
        data.eventCallback(event);
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseMovedEvent event(xPos, yPos);
        data.eventCallback(event);
    });
}

Window::~Window()
{
    shutdown();
}

void Window::onUpdate()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Window::shutdown()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::setVSync(bool enabled)
{
    if (enabled) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }

    windowData.VSync = enabled;
}
