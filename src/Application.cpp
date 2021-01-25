#include <cstdlib>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Window.h"
#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "linalg.h"
#include "errors.h"
#include "utils/Log.h"
#include "Event.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestBatch2D.h"

// F will be deduced by the compiler
template <typename T, typename E, typename F>
bool Dispatch(E event, const F& func)
{
    if (std::is_base_of<T, E>::value) {
        event.handled = func(static_cast<T&>(event));
        return true;
    }
    return false;
}

static bool shouldClose = false;

static bool keyCallback(KeyEvent event)
{
    if (event.getKey() == GLFW_KEY_ESCAPE && event.getAction() == KeyEvent::Action::PRESS) {
        shouldClose = true;
        return true;
    }
    return false;
}

static bool windowClosedCallback(WindowCloseEvent event)
{
    shouldClose = true;
    return true;
}

static void eventCallback(Event event)
{
    Dispatch<KeyEvent>(event, keyCallback);
    Dispatch<WindowCloseEvent>(event, windowClosedCallback);
}

int main(int argc, char* argv[])
{
    Log::init();

    const float WIDTH = 1024;
    const float HEIGHT = 768;
    Window window("graphX", WIDTH, HEIGHT, eventCallback);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GLDebugMessageCallback, NULL);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.getNativeWindow(), true);
    ImGui_ImplOpenGL3_Init(NULL);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    Renderer renderer;

    test::Test* currentTest = nullptr;
    test::TestMenu* testMenu = new test::TestMenu(currentTest);

    testMenu->registerTest<test::TestClearColor>("Clear Color");
    testMenu->registerTest<test::TestTexture2D>("Texture 2D");
    testMenu->registerTest<test::TestBatch2D>("Batch 2D");

    //currentTest = testMenu;
    currentTest = new test::TestBatch2D();

    while (!shouldClose) {
        renderer.clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (currentTest) {
            currentTest->onUpdate(0.0f);
            currentTest->onRender();
            ImGui::Begin("Test");
            if (currentTest != testMenu && ImGui::Button("<-")) {
                delete currentTest;
                currentTest = testMenu;
            }
            currentTest->onImGuiRender();
            ImGui::End();
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.onUpdate();
    }

    delete currentTest;
    if (currentTest != testMenu) {
        delete testMenu;
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
