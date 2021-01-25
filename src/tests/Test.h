#ifndef __TEST_H_
#define __TEST_H_

#include <string>
#include <vector>
#include <functional>
#include "../utils/Log.h"

namespace test {

class Test {
public:
    Test() { }
    virtual ~Test() { }

    virtual void onUpdate(float deltaTime) { }
    virtual void onRender() { }
    virtual void onImGuiRender() { }
};

class TestMenu : public Test {
public:
    TestMenu(Test*& currentTest);
    ~TestMenu();

    void onImGuiRender() override;

    template <typename T>
    void registerTest(const std::string& name)
    {
        INFO("Registering test {}", name);
        tests.push_back(std::make_pair(name, []() { return new T(); }));
    }

private:
    Test*& currentTest;
    std::vector<std::pair<std::string, std::function<Test*()>>> tests;
};

}

#endif // __TEST_H_
