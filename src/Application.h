#ifndef __APPLICATION_H_
#define __APPLICATION_H_

#include "Window.h"
#include <string>
#include <memory>
#include "tests/Test.h"

class Application {
public:
    ~Application();
    static Application& init(const std::string name, uint32_t width, uint32_t height);
    void run();
    void onEvent(Event& event);
    void close();

    inline Window& getWindow() { return *window; }
    inline static Application& Get() { return *instance; }
private:
    Application(const std::string name, uint32_t width, uint32_t height);
    static Application* instance;
    std::unique_ptr<Window> window;
    bool running = true;
    float lastFrameTime = 0.0f;

    test::Test* currentTest;
    test::TestMenu* testMenu;

    bool onWindowClose(WindowCloseEvent& e);
};

#endif // __APPLICATION_H_
