#include "TestLight.h"
#include <imgui/imgui.h>
#include "Test.h"
#include "../Renderer.h"

#include <numeric>
#include <sstream>

namespace test {

LightBlock::LightBlock(std::string name, GLuint program, Light light)
    : name(name)
{
    source.push_back(light);
    createUniformBlock(name, program);
}

LightBlock::LightBlock(std::string name, GLuint program, const std::vector<Light>& lights)
    : name(name)
{
    if (lights.size() > MAX_NLIGHTS) {
        throw new TooManyLightsException();
    }
    std::copy(lights.begin(), lights.end(), source.begin());
    createUniformBlock(name, program);
}

LightBlock::~LightBlock()
{
    removeBlock();
}

void LightBlock::addLight(Light light)
{
    if (source.size() == MAX_NLIGHTS) {
        throw new TooManyLightsException();
    }
    source.push_back(light);
    source[source.size() - 1].copyOffsets(source[0], (source.size() - 1) * lightStride);
}

std::vector<std::string> LightBlock::getNames() const
{
    std::vector<std::string> names = source[0].getNames();
    names.push_back(names[0]);
    for (int i = 0; i < names.size() - 1; i++) {
        names[i] = name + ".source[0]." + names[i];
    }
    names[names.size() - 1] = name + ".source[1]." + names[names.size() - 1];
    names.insert(names.begin(), name + ".nSources");

    TRACE("LightBlock names: {}", std::accumulate(names.begin(), names.end(), std::string(", ")));
    return names;
}

void LightBlock::set() const
{
    GLuint nSources = source.size();
    setData(&nSources, sizeof(nSources), nSourcesOffset);
    for (int i = 0; i < source.size(); i++) {
        source[i].set();
    }
}

void LightBlock::setOffsets(std::vector<GLint>::iterator& offsets)
{
    nSourcesOffset = *offsets;
    offsets++;
    firstLightOffset = *offsets;
    source[0].setOffsets(offsets);
    lightStride = *offsets - firstLightOffset;
    for (int i = 1; i < source.size(); i++) {
        source[i].copyOffsets(source[0], lightStride * i);
    }
}

void TestLight::writeQuad(vertex* buff, vec3 pos, float size, vec4 color)
{
    float x = pos.arr[0], y = pos.arr[1], z = pos.arr[2];
    // clang-format off
    buff[0] = vertex(x,        y,        z, color),
    buff[1] = vertex(x + size, y,        z, color),
    buff[2] = vertex(x + size, y + size, z, color),
    buff[3] = vertex(x,        y + size, z, color);
    // clang-format on
}

TestLight::TestLight()
    : Test("Light Test")
    , translationCube(vec3(-2 * size, 0, 0))
    , translationTetra(vec3(2 * size, 0, 0))
    , camera(Camera::Projection::Perspective)
{
    TRACE("Creating Light test");
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    // clang-format off
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
    // clang-format on

    VAO = std::make_unique<VertexArray>();
    indexBufferCube = std::make_unique<IndexBuffer>((GLuint*)cube_indices, 12 * 3);
    indexBufferTetra = std::make_unique<IndexBuffer>((GLuint*)tetrahedron_indices, 4 * 3);
    vertexBuffer = std::make_unique<VertexBuffer>(nullptr, MaxVertexCount * sizeof(vertex), GL_DYNAMIC_DRAW);
    VertexBufferLayout layout;
    vertex::createLayout(layout);
    VAO->addBuffer(*vertexBuffer, layout);

    shader = std::make_unique<Shader>("shaders/testLight.glsl.vert", "shaders/testLight.glsl.geom", "shaders/testLight.glsl.frag");
    shader->bind();

    Light light(defaultAmbient, defaultDirect, defaultLightPosition);
    lights = std::make_unique<LightBlock>("LightBlock", shader->getRendererId(), light);

    /*
    vertex background[4];
    writeQuad(background, vec3(-1, -1, -1), 2, vec4(0, 0, 0, 0), 1);
    vertexBuffer->sendData(background, sizeof(background));
    */
}

TestLight::~TestLight()
{
}

void TestLight::onUpdate(float deltatime)
{
    camera.onUpdate(deltatime);
    vertex vertices[8];
    writeQuad(vertices, vec3(-1 * size, -1 * size, -2), 2 * size, vec4(1.0f, 0.0f, 0.0f, 1.0f));
    writeQuad(vertices + 4, vec3(-1 * size, -1 * size, -2 - 2 * size), 2 * size, vec4(0.0f, 0.0f, 1.0f, 1.0f));
    vertexBuffer->sendData(vertices, sizeof(vertices));
}

void TestLight::onRender()
{
    Renderer renderer;
    renderer.clear();

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    mat4 model_cube = mat4::Translation(translationCube.arr);
    mat4 model_tetra = mat4::Translation(translationTetra.arr);
    shader->bind();

    vec4 eyepos = vec4::point(camera.getPosition());
    shader->setUniform4f("eyepos", eyepos.arr);
    lights->set();

    mat4 MVPTransform = camera.getViewProjectionMatrix() * model_cube;
    mat4 modelTransform = model_cube;
    shader->setUniformMat4f("MVPTransform", MVPTransform.arr);
    shader->setUniformMat4f("ModelTransform", modelTransform.arr);
    renderer.draw(*VAO, *indexBufferCube, *shader, 12 * 3);

    MVPTransform = camera.getViewProjectionMatrix() * model_tetra;
    modelTransform = model_tetra;
    shader->setUniformMat4f("MVPTransform", MVPTransform.arr);
    shader->setUniformMat4f("ModelTransform", modelTransform.arr);
    renderer.draw(*VAO, *indexBufferTetra, *shader, 4 * 3);
}

void TestLight::onImGuiRender()
{
    if (ImGui::BeginTabBar("Controls")) {
        if (ImGui::BeginTabItem("Objects")) {
            ImGui::DragFloat2("Cube position", translationCube.arr, 0.1f);
            ImGui::DragFloat2("Tetrahedron position", translationTetra.arr, 0.1f);
            ImGui::DragFloat("Model size", &size, 0.05f);
            vec3 position = camera.getPosition();
            ImGui::Text("Camera: position = (%.1f, %.1f, %.1f), aspect = %.3f, zoom = %.1f",
                position.arr[0], position.arr[1], position.arr[2],
                camera.getAspectRatio(), camera.getZoomLevel());
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Lights")) {
            if (ImGui::Button("Create")) {
                Light light(defaultAmbient, defaultDirect, defaultLightPosition);
                lights->addLight(light);
            }
            std::vector<Light>& source = lights->getSource();
            for (int i = 0; i < source.size(); i++) {
                ImGui::PushID(i);
                std::ostringstream name;
                name << "Light " << i << " position";
                ImGui::DragFloat3(name.str().c_str(), source[i].getPositionRef().arr, 0.1);
                ImGui::SameLine();
                ImGui::Checkbox("Inf dist", &(infDistLight[i]));
                if (infDistLight[i]) {
                    source[i].getPositionRef().arr[3] = 0;
                } else {
                    source[i].getPositionRef().arr[3] = 1;
                }
                ImGui::PopID();
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void TestLight::onEvent(Event& e)
{
    camera.onEvent(e);
}

}
