#include "Application.h"
#include <cstdlib>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Window.h"
#include "Renderer.h"
#include "utils/Log.h"
#include "Event.h"
#include "Input.h"

#include "tests/TestImGui.h"
#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestBatch2D.h"
#include "tests/TestCamera.h"
#include "tests/Test3D.h"
#include "tests/TestLight.h"
#include "tests/TestDrawFlat.h"

Application* Application::instance = nullptr;

Application::Application(const std::string name, uint32_t width, uint32_t height)
{
    TRACE("Creating Application {} {} {}", name, width, height);
    window = std::make_unique<Window>(name, width, height, BIND_EVENT_FN(Application::onEvent));

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window->getNativeWindow(), true);
    ImGui_ImplOpenGL3_Init(NULL);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    Input::init(width, height);

    TRACE("Initializing test menu");
    testMenu = new test::TestMenu(currentTest);
    currentTest = new test::TestDrawFlat();

    testMenu->registerTest<test::TestImGui>("ImGui");
    //testMenu->registerTest<test::TestClearColor>("Clear Color");
    //testMenu->registerTest<test::TestTexture2D>("Texture 2D");
    testMenu->registerTest<test::TestBatch2D>("Batch 2D");
    testMenu->registerTest<test::TestCamera>("Camera");
    testMenu->registerTest<test::Test3D>("Test 3D");
    testMenu->registerTest<test::TestLight>("Test Light");
    testMenu->registerTest<test::TestDrawFlat>("Test Draw Flat");

}

Application::~Application()
{
    delete currentTest;
    if (currentTest != testMenu) {
        delete testMenu;
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

Application& Application::init(const std::string name, uint32_t width, uint32_t height)
{
    TRACE("Application INIT");
    if (instance == nullptr) {
        TRACE("Creating Application");
        instance = new Application(name, width, height);
    }
    return *instance;
}

void Application::run()
{
    Renderer renderer;
    TRACE("Application started");
    while (running) {
        renderer.clear();
        glClearColor(0, 0, 0, 0);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (currentTest) {
            float time = (float)glfwGetTime();
            float timestep = time - lastFrameTime;
            lastFrameTime = time;
            currentTest->onUpdate(timestep);
            currentTest->onRender();
            ImGui::Begin(currentTest->getName().c_str());
            Input::onImGui();
            if (currentTest != testMenu && ImGui::Button("<-")) {
                delete currentTest;
                currentTest = testMenu;
            }
            currentTest->onImGuiRender();
            ImGui::End();
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window->onUpdate();
    }
}

static bool keyCallback(KeyEvent& event)
{
    if (event.getKey() == GLFW_KEY_ESCAPE && event.getAction() == KeyEvent::Action::PRESS) {
        Application::Get().close();
        return true;
    }
    return false;
}

bool Application::onWindowClose(WindowCloseEvent& e)
{
    TRACE("windowClosedCallback");
    close();
    return true;
}


void Application::onEvent(Event& event)
{
    DispatchEvent<KeyEvent>(event, keyCallback);
    DispatchEvent<WindowCloseEvent>(event, BIND_EVENT_FN(Application::onWindowClose));
    Input::onEvent(event);
    if(currentTest) {
        currentTest->onEvent(event);
    }
}

void Application::close()
{
    running = false;
}
