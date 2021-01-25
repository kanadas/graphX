#include <cstdlib>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Application.h"
#include "Window.h"
#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "linalg.h"
#include "utils/errors.h"
#include "utils/Log.h"
#include "Event.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestBatch2D.h"

int main(int argc, char* argv[])
{
    Log::init();
    const float WIDTH = 1024;
    const float HEIGHT = 768;
    Application& app = Application::init("graphX", WIDTH, HEIGHT);
    app.run();
    return 0;
}
