#ifndef __TESTCAMERA_H_
#define __TESTCAMERA_H_

#include "glad/glad.h"
#include "Test.h"
#include "utils/linalg.h"
#include "scene/Texture.h"
#include "buffers/VertexBuffer.h"
#include "buffers/VertexBufferLayout.h"
#include "Event.h"
#include "scene/Camera.h"

#include <memory>

namespace test {

class TestCamera : public Test {
private:
    std::unique_ptr<VertexArray> VAO;
    std::unique_ptr<IndexBuffer> indexBuffer;
    std::unique_ptr<VertexBuffer> vertexBuffer;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Texture> textureA;
    std::unique_ptr<Texture> textureB;
    vec3 translation;
    GLuint indexCount = 0;
    GLfloat size = 0.2f;

    Camera camera;

    const GLuint MaxQuadCount = 1000;
    const GLuint MaxVertexCount = MaxQuadCount * 4;
    const GLuint MaxIndexCount = MaxQuadCount * 6;

public:
    TestCamera();
    ~TestCamera();

    void onUpdate(float deltatime) override;
    void onRender() override;
    void onImGuiRender() override;
    virtual void onEvent(Event& e) override;
};
}

#endif // __TESTCAMERA_H_
