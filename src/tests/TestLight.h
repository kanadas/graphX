#ifndef __TESTLIGHT_H_
#define __TESTLIGHT_H_

#include "glad/glad.h"
#include "Test.h"
#include "../linalg.h"
#include "../VertexBuffer.h"
#include "../VertexBufferLayout.h"
#include "../VertexArray.h"
#include "../IndexBuffer.h"
#include "../Shader.h"
#include "../Event.h"
#include "../Camera.h"
#include "../Light.h"

#include <memory>

namespace test {

class LightBlock : public UniformBlock {
public:
    LightBlock() = delete; //Enforce at least one light
    LightBlock(std::string name, GLuint program, Light light);
    LightBlock(std::string name, GLuint program, const std::vector<Light>& lights);
    ~LightBlock();
    void addLight(Light light);
    std::vector<Light>& getSource() { return source; }

    virtual std::vector<std::string> getNames() const override;
    virtual void set() const override;
    virtual void setOffsets(std::vector<GLint>::iterator& offsets) override;

    static const GLint MAX_NLIGHTS = 8; //hardcoded in shader
private:
    std::string name;
    std::vector<Light> source;
    GLint nSourcesOffset;
    GLint firstLightOffset;
    GLint lightStride;
};

class TestLight : public Test {
private:
    std::unique_ptr<VertexArray> VAO;
    std::unique_ptr<IndexBuffer> indexBufferCube;
    std::unique_ptr<IndexBuffer> indexBufferTetra;
    std::unique_ptr<VertexBuffer> vertexBuffer;
    std::unique_ptr<Shader> shader;
    GLfloat size = 0.2f;
    vec3 translationCube;
    vec3 translationTetra;
    GLuint indexCount = 12 * 3;
    bool infDistLight[LightBlock::MAX_NLIGHTS] = { true };

    Camera camera;
    std::unique_ptr<LightBlock> lights;
    vec4 defaultAmbient = { 0.2, 0.2, 0.2, 1.0 };
    vec4 defaultDirect = { 0.8, 0.8, 0.8, 1.0 };
    vec4 defaultLightPosition = { 0.0, 1.0, 1.0, 0.0 }; //infinetely far

    const GLuint MaxQuadCount = 1000;
    const GLuint MaxVertexCount = MaxQuadCount * 4;
    const GLuint MaxIndexCount = MaxQuadCount * 6;

    struct vertex {
        GLfloat position[3];
        GLfloat color[4];

        vertex() { }

        vertex(GLfloat position[3], GLfloat color[4])
        {
            memcpy(this->position, position, 3 * sizeof(GLfloat));
            memcpy(this->color, color, 4 * sizeof(GLfloat));
        }

        vertex(GLfloat x, GLfloat y, GLfloat z, vec4 color)
            : position { x, y, z }
        {
            memcpy(this->color, color.arr, 4 * sizeof(GLfloat));
        }

        static void createLayout(VertexBufferLayout& layout)
        {
            layout.push(3, GL_FLOAT); //position
            layout.push(4, GL_FLOAT); //color
        }
    };

    static void writeQuad(vertex* buff, vec3 pos, float size, vec4 color);

public:
    TestLight();
    ~TestLight();

    void onUpdate(float deltatime) override;
    void onRender() override;
    void onImGuiRender() override;
    virtual void onEvent(Event& e) override;
};

class TooManyLightsException {};

}

#endif // __TESTLIGHT_H_
