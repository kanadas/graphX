#include "Test3D.h"
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

Test3D::Test3D()
    : translationCube(vec3(-2 * size, 0, 0))
    , translationTetra(vec3(2 * size, 0, 0))
    , camera(Camera::Projection::Ortographic)
{
    TRACE("Creating 3D test");
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    GLuint cube_indices[12*3] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        0, 1, 5, 5, 4, 0,
        1, 2, 6, 6, 5, 1,
        2, 3, 7, 7, 6, 2,
        3, 0, 4, 4, 7, 3,
    };

    GLuint tetrahedron_indices[4*3] = {
        1, 3, 4,
        1, 6, 4,
        3, 4, 6,
        3, 1, 6,
    };

    VAO = std::make_unique<VertexArray>();
    indexBufferCube = std::make_unique<IndexBuffer>((GLuint*)cube_indices, 12*3);
    indexBufferTetra = std::make_unique<IndexBuffer>((GLuint*)tetrahedron_indices, 4*3);
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

    /*
    vertex background[4];
    writeQuad(background, vec3(-1, -1, -1), 2, vec4(0, 0, 0, 0), 1);
    vertexBuffer->sendData(background, sizeof(background));
    */
}

Test3D::~Test3D()
{
}

void Test3D::onUpdate(float deltatime)
{
    camera.onUpdate(deltatime);
    vertex vertices[8];
    writeQuad(vertices, vec3(-1 * size, -1 * size, -2), 2*size, vec4(1.0f, 0.0f, 0.0f, 1.0f), -1);
    writeQuad(vertices + 4, vec3(-1 * size, -1 * size, -2 - 2*size), 2*size, vec4(0.0f, 0.0f, 1.0f, 1.0f), -1);
    vertexBuffer->sendData(vertices, sizeof(vertices));
}

void Test3D::onRender()
{
    Renderer renderer;
    renderer.clear();
    textureA->bind(0);
    textureB->bind(1);

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    mat4 model_cube = mat4::Translation(translationCube.arr);
    mat4 model_tetra = mat4::Translation(translationTetra.arr);

    shader->bind();
    mat4 mvp = camera.getViewProjectionMatrix() * model_cube;
    shader->setUniformMat4f("u_MVP", mvp.arr);
    renderer.draw(*VAO, *indexBufferCube, *shader, 12 * 3);

    mvp = camera.getViewProjectionMatrix() * model_tetra;
    shader->setUniformMat4f("u_MVP", mvp.arr);
    renderer.draw(*VAO, *indexBufferTetra, *shader, 4 * 3);
}

void Test3D::onImGuiRender()
{
    ImGui::DragFloat2("Cube position", translationCube.arr, 0.1f);
    ImGui::DragFloat2("Tetrahedron position", translationTetra.arr, 0.1f);
    ImGui::DragFloat("Model size", &size, 0.05f);
    vec3 position = camera.getPosition();
    ImGui::Text("Camera: position = (%.1f, %.1f, %.1f), rotation = %.3f, aspect = %.3f, zoom = %.1f", position.arr[0], position.arr[1], position.arr[2], camera.getRotation(), camera.getAspectRatio(), camera.getZoomLevel());
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void Test3D::onEvent(Event& e)
{
    camera.onEvent(e);
}
}
