#include "Application.h"
#include "utils/Log.h"

int main(int argc, char* argv[])
{
    Log::init();
    const float WIDTH = 1024;
    const float HEIGHT = 768;
    Application& app = Application::init("graphX", WIDTH, HEIGHT);
    app.run();
    return 0;
}
