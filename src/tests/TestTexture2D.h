#ifndef __TESTTEXTURE2D_H_
#define __TESTTEXTURE2D_H_

#include "glad/glad.h"
#include "Test.h"
#include "../linalg.h"
#include "../Texture.h"
#include "../VertexBuffer.h"
#include "../VertexBufferLayout.h"

#include <memory>

namespace test {

class TestTexture2D : public Test {
private:
    std::unique_ptr<VertexArray> VAO;
    std::unique_ptr<IndexBuffer> indexBuffer;
    std::unique_ptr<VertexBuffer> vertexBuffer;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Texture> texture;
    vec3 translationA;
    vec3 translationB;
    mat4 proj;
    mat4 view;

public:
    TestTexture2D();
    ~TestTexture2D();

    void onUpdate(float deltatime) override;
    void onRender() override;
    void onImGuiRender() override;
};
}

#endif // __TESTTEXTURE2D_H_
