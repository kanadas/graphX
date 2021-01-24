#include "TestTexture2D.h"
#include <imgui/imgui.h>
#include "Test.h"
#include "../Renderer.h"

namespace test {

TestTexture2D::TestTexture2D()
    : translationA(200, 200, 0)
    , translationB(400, 200, 0)
    , proj(mat4::Ortho(0.0f, 640.0f, 0.0f, 480.0f, -1.0f, 1.0f))
    , view(mat4::Translation(0.0f, 0.0f, 0.0f))
{
    // Clang-format off
    // position, texture pos
    float positions[] = {
        -50.0f, -50.0f, 0.0f, 0.0f, // 0
         50.0f, -50.0f, 1.0f, 0.0f, //1
         50.0f,  50.0f, 1.0f, 1.0f, //2
        -50.0f,  50.0f, 0.0f, 1.0f //3
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    // clang-format on
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    VAO = std::make_unique<VertexArray>();
    indexBuffer = std::make_unique<IndexBuffer>(indices, 6);
    vertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.push(2, GL_FLOAT); //position
    layout.push(2, GL_FLOAT); //texture pos
    VAO->addBuffer(*vertexBuffer, layout);

    shader = std::make_unique<Shader>("shaders/project.glsl.vert", "shaders/project.glsl.frag");
    shader->bind();

    texture = std::make_unique<Texture>("../resources/logo.png");
    shader->setUniform1i("u_Texture", 0);
}

TestTexture2D::~TestTexture2D()
{
}

void TestTexture2D::onUpdate(float deltatime)
{
}

void TestTexture2D::onRender()
{
    Renderer renderer;
    texture->bind();

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    {
        mat4 model = mat4::Translation(translationA);
        mat4 mvp = proj * view * model;
        shader->bind();
        shader->setUniformMat4f("u_MVP", mvp.arr);
        renderer.draw(*VAO, *indexBuffer, *shader);
    }
    {
        mat4 model = mat4::Translation(translationB);
        mat4 mvp = proj * view * model;
        shader->setUniformMat4f("u_MVP", mvp.arr);
        renderer.draw(*VAO, *indexBuffer, *shader);
    }
}

void TestTexture2D::onImGuiRender()
{
    ImGui::SliderFloat3("TranslationA", translationA.arr, 0.0f, 640.0f);
    ImGui::SliderFloat3("TranslationB", translationB.arr, 0.0f, 640.0f);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
}
