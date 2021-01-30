#ifndef __TEST_H_
#define __TEST_H_

#include <string>
#include <vector>
#include <functional>
#include "../utils/Log.h"
#include "../Event.h"

namespace test {

class Test {
public:
    Test(std::string name) : name(name) { }
    virtual ~Test() { }

    std::string getName() { return name; }

    virtual void onUpdate(float deltaTime) { }
    virtual void onRender() { }
    virtual void onImGuiRender() { }
    virtual void onEvent(Event& e) { }
private:
    std::string name;
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
