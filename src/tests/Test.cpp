#include "Test.h"
#include <imgui/imgui.h>

namespace test {

TestMenu::TestMenu(Test*& currentTest)
    : currentTest(currentTest)
{
}

TestMenu::~TestMenu()
{
}

void TestMenu::onImGuiRender()
{
    for (auto& test: tests) {
        if (ImGui::Button(test.first.c_str())) {
            currentTest = test.second();
        }
    }
}

}
