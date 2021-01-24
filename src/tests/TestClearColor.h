#ifndef __TESTCLEARCOLOR_H_
#define __TESTCLEARCOLOR_H_

#include "glad/glad.h"
#include "Test.h"

namespace test {

    class TestClearColor: public Test
    {
    private:
        GLfloat clearColor[4];
    public:
        TestClearColor();
        ~TestClearColor();

        void onUpdate(float deltatime) override;
        void onRender() override;
        void onImGuiRender() override;
    };
}

#endif // __TESTCLEARCOLOR_H_
