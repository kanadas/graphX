#ifndef __TESTIMGUI_H_
#define __TESTIMGUI_H_

#include "glad/glad.h"
#include "Test.h"

namespace test {

    class TestImGui: public Test
    {
    public:
        TestImGui();
        ~TestImGui();

        void onUpdate(float deltatime) override;
        void onRender() override;
        void onImGuiRender() override;
    };
}

#endif // __TESTIMGUI_H_
