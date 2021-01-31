#include "TestDrawFlat.h"
#include <imgui/imgui.h>
#include "Test.h"
#include "Renderer.h"

#include <numeric>
#include <sstream>

namespace test {

TestDrawFlat::LightBlock::LightBlock(std::string name, GLuint program, Light light)
    : name(name)
{
    source.push_back(light);
    createUniformBlock(name, program);
}

TestDrawFlat::LightBlock::LightBlock(std::string name, GLuint program, const std::vector<Light>& lights)
    : name(name)
{
    if (lights.size() > MAX_NLIGHTS) {
        throw new TooManyLightsException();
    }
    std::copy(lights.begin(), lights.end(), source.begin());
    createUniformBlock(name, program);
}

TestDrawFlat::LightBlock::~LightBlock()
{
    removeBlock();
}

void TestDrawFlat::LightBlock::addLight(Light light)
{
    if (source.size() == MAX_NLIGHTS) {
        throw new TooManyLightsException();
    }
    source.push_back(light);
    source[source.size() - 1].copyOffsets(source[0], (source.size() - 1) * lightStride);
}

std::vector<std::string> TestDrawFlat::LightBlock::getNames() const
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

void TestDrawFlat::LightBlock::set() const
{
    GLuint nSources = source.size();
    setData(&nSources, sizeof(nSources), nSourcesOffset);
    for (int i = 0; i < source.size(); i++) {
        source[i].set();
    }
}

void TestDrawFlat::LightBlock::setOffsets(std::vector<GLint>::iterator& offsets)
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

void TestDrawFlat::writeQuad(int modelIdx, vec3 pos, float size, vec4 color)
{
    float x = pos.arr[0], y = pos.arr[1], z = pos.arr[2];
    // clang-format off
    models[modelIdx].addVertex(Vertex::createData(vec3(x,        y,        z), color));
    models[modelIdx].addVertex(Vertex::createData(vec3(x + size, y,        z), color));
    models[modelIdx].addVertex(Vertex::createData(vec3(x + size, y + size, z), color));
    models[modelIdx].addVertex(Vertex::createData(vec3(x,        y + size, z), color));
    // clang-format on
}

TestDrawFlat::TestDrawFlat()
    : Test("DrawFlat Test")
    , translation(vec3(0, 0, -2))
    , camera(Camera::Projection::Perspective)
{
    TRACE("Creating DrawFlat test");

    // clang-format off
    GLuint cube_indices[12*3] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        0, 1, 5, 5, 4, 0,
        1, 2, 6, 6, 5, 1,
        2, 3, 7, 7, 6, 2,
        3, 0, 4, 4, 7, 3,
    };
    // clang-format on

    models.push_back(Model<Vertex>("Cube"));
    writeQuad(0, vec3(-0.2, -0.2, 0.2), 0.4, vec4(1.0f, 0.0f, 0.0f, 1.0f));
    writeQuad(0, vec3(-0.2, -0.2, -0.2), 0.4, vec4(0.0f, 0.0f, 1.0f, 1.0f));
    for (int i = 0; i < 12*3; i++) {
        models[0].addIndex(cube_indices[i]);
    }
    //models[0].setTranslation(vec3(-0.5, 0, 0));

    shader = std::make_unique<Shader>("shaders/testLight.glsl.vert", "shaders/testLight.glsl.geom", "shaders/testLight.glsl.frag");
    shader->bind();

    Light light(defaultAmbient, defaultDirect, defaultLightPosition);
    lights = std::make_unique<LightBlock>("LightBlock", shader->getRendererId(), light);
}

TestDrawFlat::~TestDrawFlat()
{
}

void TestDrawFlat::onUpdate(float deltatime)
{
    camera.onUpdate(deltatime);
    for (int i = 0; i < models.size(); i++) {
        models[i].sendData();
    }
}

void TestDrawFlat::onRender()
{
    Renderer renderer;
    mat4 model = mat4::Translation(translation.arr);
    vec4 eyepos = vec4::point(camera.getPosition() + translation * -1);

    shader->bind();
    shader->setUniform4f("eyepos", eyepos.arr);
    lights->set();
    for (int i = 0; i < models.size(); i++) {
        mat4 modelTransform = model * models[i].getTransform();
        mat4 MVPTransform = camera.getViewProjectionMatrix() * modelTransform;
        shader->setUniformMat4f("ModelTransform", modelTransform.arr);
        shader->setUniformMat4f("MVPTransform", MVPTransform.arr);
        models[i].draw(shader);
    }
}

void TestDrawFlat::onImGuiRender()
{
    if (ImGui::BeginTabBar("Controls")) {
        if (ImGui::BeginTabItem("Flat Models")) {
            if (ImGui::CollapsingHeader("New Model"))
            {
            }
            for (int i = 0; i < models.size(); i++) {
                ImGui::PushID(i);
                if (ImGui::CollapsingHeader(models[i].getName().c_str()))
                {
                    ImGui::DragFloat3("Position", models[i].getTranslation().arr, 0.02);
                    ImGui::DragFloat3("Rotation", models[i].getRotationPerAxis().arr, 0.1);

                    uint32_t nvert = models[i].getNVertices();
                    if (ImGui::TreeNode("Vertices", "Vertices (%d)", nvert))
                    {
                        for (int j = 0; j < nvert; j++) {
                            ImGui::PushID(j);
                            Vertex v = models[i].getVertex(j);
                            ImGui::DragFloat3("##Vert", v.getPosition(), 0.02);
                            ImGui::SameLine();
                            ImGui::ColorEdit4("##VertColor", v.getColor(), ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                            ImGui::PopID();
                        }
                        ImGui::TreePop();
                    }
                }
                ImGui::PopID();
            }
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
    vec3 position = camera.getPosition();
    ImGui::Text("Camera: position = (%.1f, %.1f, %.1f), aspect = %.3f, zoom = %.1f",
        position.arr[0], position.arr[1], position.arr[2], camera.getAspectRatio(), camera.getZoomLevel());
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void TestDrawFlat::onEvent(Event& e)
{
    camera.onEvent(e);
}

}
