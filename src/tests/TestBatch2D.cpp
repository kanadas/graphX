#include "TestBatch2D.h"
#include <imgui/imgui.h>
#include "Test.h"
#include "../Renderer.h"

namespace test {

struct vertex {
    GLfloat position[2];
    GLfloat color[4];
    GLfloat texCoords[2];
    GLint texID;

    vertex() { }

    vertex(GLfloat position[2], GLfloat color[4], GLfloat texCoords[2], GLint texID)
        : texID(texID)
    {
        memcpy(this->position, position, 2 * sizeof(GLfloat));
        memcpy(this->color, color, 4 * sizeof(GLfloat));
        memcpy(this->texCoords, texCoords, 2 * sizeof(GLfloat));
    }

    vertex(GLfloat x, GLfloat y, vec4 color, GLfloat texX, GLfloat texY, GLint texID)
        : position { x, y }
        , texCoords { texX, texY }
        , texID(texID)
    {
        memcpy(this->color, color.arr, 4 * sizeof(GLfloat));
    }

    static VertexBufferLayout createLayout()
    {
        VertexBufferLayout layout;
        layout.push(2, GL_FLOAT); //position
        layout.push(4, GL_FLOAT); //color
        layout.push(2, GL_FLOAT); //texture
        layout.push(1, GL_INT); //texture ID
        return layout;
    }
};


static void writeQuad(vertex* buff, GLfloat x, GLfloat y, float size, vec4 color, GLint textureID)
{
    buff[0] = vertex(x, y, color, 0.0f, 0.0f, textureID),
    buff[1] = vertex(x + size, y, color, 1.0f, 0.0f, textureID),
    buff[2] = vertex(x + size, y + size, color, 1.0f, 1.0f, textureID),
    buff[3] = vertex(x, y + size, color, 0.0f, 1.0f, textureID);
}

TestBatch2D::TestBatch2D()
    : translation(vec3(100, 100, 0))
    //, positions { { -50, -50 }, { 100, -50 }, { 250, -50 } }
    , proj(mat4::Ortho(0.0f, 640.0f, 0.0f, 480.0f, -1.0f, 1.0f))
    , view(mat4::Translation(0.0f, 0.0f, 0.0f))
    , indexCount(0)
    , size(50)
{
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
    VertexBufferLayout layout = vertex::createLayout();
    VAO->addBuffer(*vertexBuffer, layout);

    shader = std::make_unique<Shader>("shaders/testBatch/vertex.glsl", "shaders/testBatch/fragment.glsl");
    shader->bind();

    textureA = std::make_unique<Texture>("../resources/logo.png");
    textureB = std::make_unique<Texture>("../resources/texture.jpg");
    int samplers[2] = { 0, 1 };
    shader->setUniform1iv("u_Textures", 2, samplers);

    vertex background[4];
    writeQuad(background, -480, -480, 960, vec4(0,0,0,0), 1);
    vertexBuffer->sendData(background, sizeof(background));
}

TestBatch2D::~TestBatch2D()
{
}

void TestBatch2D::onUpdate(float deltatime)
{
    vec4 color = { 1.0f, 0.0f, 0.0f, 1.0f };
    vertex vertices[5*5*4];
    uint32_t offset = 0;
    indexCount = 6; //background
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            writeQuad(vertices + offset, x * size, y * size, size, color, ((x + y) % 2) - 1);
            offset += 4;
            indexCount += 6;
        }
    }
    vertexBuffer->sendData(vertices, sizeof(vertices), 4 * sizeof(vertex)); //background
}

void TestBatch2D::onRender()
{
    Renderer renderer;
    textureA->bind(0);
    textureB->bind(1);

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    mat4 model = mat4::Translation(translation.arr);
    mat4 mvp = proj * view * model;
    shader->bind();
    shader->setUniformMat4f("u_MVP", mvp.arr);
    renderer.draw(*VAO, *indexBuffer, *shader, indexCount);
}

void TestBatch2D::onImGuiRender()
{
    ImGui::DragFloat2("Checkboard position", translation.arr, 10.0f);
    ImGui::DragFloat("Checkboard size", &size, 1.0f);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
}
