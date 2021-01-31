#ifndef __TESTBATCH2D_H_
#define __TESTBATCH2D_H_

#include "glad/glad.h"
#include "Test.h"
#include "utils/linalg.h"
#include "scene/Texture.h"
#include "buffers/VertexBuffer.h"
#include "buffers/VertexBufferLayout.h"

#include <memory>

namespace test {

class TestBatch2D : public Test {
private:
    std::unique_ptr<VertexArray> VAO;
    std::unique_ptr<IndexBuffer> indexBuffer;
    std::unique_ptr<VertexBuffer> vertexBuffer;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Texture> textureA;
    std::unique_ptr<Texture> textureB;
    //float positions[3][2];
    vec3 translation;
    mat4 proj;
    mat4 view;
    GLuint indexCount;
    GLfloat size;

    const GLuint MaxQuadCount = 1000;
    const GLuint MaxVertexCount = MaxQuadCount * 4;
    const GLuint MaxIndexCount = MaxQuadCount * 6;

public:
    TestBatch2D();
    ~TestBatch2D();

    void onUpdate(float deltatime) override;
    void onRender() override;
    void onImGuiRender() override;
};
}

#endif // __TESTBATCH2D_H_
