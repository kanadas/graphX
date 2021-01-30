#include "TestImGui.h"
#include <imgui/imgui.h>

namespace test {

TestImGui::TestImGui()
    : Test("ImGui Test")
{
}

TestImGui::~TestImGui()
{
}

void TestImGui::onUpdate(float deltatime)
{
}

void TestImGui::onRender()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void TestImGui::onImGuiRender()
{
    ImGui::ShowDemoWindow();
}

}
