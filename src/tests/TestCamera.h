#ifndef __TESTCAMERA_H_
#define __TESTCAMERA_H_

#include "glad/glad.h"
#include "Test.h"
#include "../linalg.h"
#include "../Texture.h"
#include "../VertexBuffer.h"
#include "../VertexBufferLayout.h"
#include "../Event.h"
#include "../Camera.h"

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
    GLuint indexCount;
    GLfloat size;

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
