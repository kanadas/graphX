#include "TestCamera.h"
#include <imgui/imgui.h>
#include "Test.h"
#include "../Renderer.h"

namespace test {

struct vertex {
    GLfloat position[3];
    GLfloat color[4];
    GLfloat texCoords[2];
    GLint texID;

    vertex() { }

    vertex(GLfloat position[3], GLfloat color[4], GLfloat texCoords[2], GLint texID)
        : texID(texID)
    {
        memcpy(this->position, position, 3 * sizeof(GLfloat));
        memcpy(this->color, color, 4 * sizeof(GLfloat));
        memcpy(this->texCoords, texCoords, 2 * sizeof(GLfloat));
    }

    vertex(GLfloat x, GLfloat y, GLfloat z, vec4 color, GLfloat texX, GLfloat texY, GLint texID)
        : position { x, y, z }
        , texCoords { texX, texY }
        , texID(texID)
    {
        memcpy(this->color, color.arr, 4 * sizeof(GLfloat));
    }

    static void createLayout(VertexBufferLayout& layout)
    {
        layout.push(3, GL_FLOAT); //position
        layout.push(4, GL_FLOAT); //color
        layout.push(2, GL_FLOAT); //texture
        layout.push(1, GL_INT); //texture ID
    }

    static uint32_t size()
    {
        return sizeof(position) + sizeof(color) + sizeof(texCoords) + sizeof(texID);
    }

    static uint8_t* writeBuffer(const vertex* arr, uint32_t n)
    {
        uint32_t size = vertex::size() * n;
        uint8_t* buff = new uint8_t[size];
        uint32_t offset = 0;
        for (int i = 0; i < n; i++) {
            memcpy(buff + offset, arr[i].position, sizeof(arr[i].position));
            offset += sizeof(arr[i].position);
            memcpy(buff + offset, arr[i].color, sizeof(arr[i].color));
            offset += sizeof(arr[i].color);
            memcpy(buff + offset, arr[i].texCoords, sizeof(arr[i].texCoords));
            offset += sizeof(arr[i].texCoords);
            memcpy(buff + offset, &arr[i].texID, sizeof(arr[i].texID));
            offset += sizeof(arr[i].texID);
        }
        return buff;
    }
};

static void writeQuad(vertex* buff, vec3 pos, float size, vec4 color, GLint textureID)
{
    float x = pos.arr[0], y = pos.arr[1], z = pos.arr[2];
    // clang-format off
    buff[0] = vertex(x,        y,        z, color, 0.0f, 0.0f, textureID),
    buff[1] = vertex(x + size, y,        z, color, 1.0f, 0.0f, textureID),
    buff[2] = vertex(x + size, y + size, z, color, 1.0f, 1.0f, textureID),
    buff[3] = vertex(x,        y + size, z, color, 0.0f, 1.0f, textureID);
    // clang-format on
}

TestCamera::TestCamera()
    : Test("Camera Test")
    , translation(vec3(0, 0, 0))
    , camera(Camera::Projection::Ortographic)
{
    TRACE("Creating camera test");
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    GLuint indices[MaxIndexCount];
    uint32_t offset = 0;
    for (int i = 0; i < MaxIndexCount; i += 6) {
        indices[i + 0] = 0 + offset;
        indices[i + 1] = 1 + offset;
        indices[i + 2] = 2 + offset;
        indices[i + 3] = 2 + offset;
        indices[i + 4] = 3 + offset;
        indices[i + 5] = 0 + offset;
        offset += 4;
    }

    VAO = std::make_unique<VertexArray>();
    indexBuffer = std::make_unique<IndexBuffer>((GLuint*)indices, MaxIndexCount);
    vertexBuffer = std::make_unique<VertexBuffer>(nullptr, MaxVertexCount * sizeof(vertex), GL_DYNAMIC_DRAW);
    VertexBufferLayout layout;
    vertex::createLayout(layout);
    VAO->addBuffer(*vertexBuffer, layout);

    shader = std::make_unique<Shader>("shaders/testBatch/vertex.glsl", "shaders/testBatch/fragment.glsl");
    shader->bind();

    textureA = std::make_unique<Texture>("../resources/logo.png");
    textureB = std::make_unique<Texture>("../resources/texture.jpg");
    int samplers[2] = { 0, 1 };
    shader->setUniform1iv("u_Textures", 2, samplers);

    vertex background[4];
    writeQuad(background, vec3(-1, -1, -1), 2, vec4(0, 0, 0, 0), 1);
    vertexBuffer->sendData(background, sizeof(background));
}

TestCamera::~TestCamera()
{
}

void TestCamera::onUpdate(float deltatime)
{
    camera.onUpdate(deltatime);
    vec4 color = { 1.0f, 0.0f, 0.0f, 1.0f };
    vertex vertices[4 * 4 * 4];
    uint32_t offset = 0;
    indexCount = 6; //background
    for (int y = -2; y < 2; y++) {
        for (int x = -2; x < 2; x++) {
            writeQuad(vertices + offset, vec3(x * size, y * size, -1.5), size, color, ((x + y + 6) % 2) - 1);
            offset += 4;
            indexCount += 6;
        }
    }
    //uint8_t* buff = vertex::writeBuffer(vertices, 4*4*4);
    vertexBuffer->sendData(vertices, sizeof(vertices), sizeof(vertex) * 4);// 4*4*4*vertex::size());
    //delete[] buff;
}

void TestCamera::onRender()
{
    Renderer renderer;
    renderer.clear();
    textureA->bind(0);
    textureB->bind(1);

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    mat4 model = mat4::Translation(translation.arr);
    mat4 mvp = camera.getViewProjectionMatrix() * model;

    shader->bind();
    shader->setUniformMat4f("u_MVP", mvp.arr);
    renderer.draw(*VAO, *indexBuffer, *shader, indexCount);
}

void TestCamera::onImGuiRender()
{
    ImGui::DragFloat2("Checkboard position", translation.arr, 0.1f);
    ImGui::DragFloat("Checkboard size", &size, 0.05f);
    vec3 position = camera.getPosition();
    ImGui::Text("Camera: position = (%.1f, %.1f, %.1f), rotation = %.3f, aspect = %.3f, zoom = %.1f", position.arr[0], position.arr[1], position.arr[2], camera.getRotation(), camera.getAspectRatio(), camera.getZoomLevel());
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void TestCamera::onEvent(Event& e)
{
    camera.onEvent(e);
}
}
