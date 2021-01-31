#ifndef __TESTDRAWFLAT_H_
#define __TESTDRAWFLAT_H_

#include "glad/glad.h"
#include "Test.h"
#include "utils/linalg.h"
#include "buffers/VertexBuffer.h"
#include "buffers/VertexBufferLayout.h"
#include "buffers/VertexArray.h"
#include "buffers/IndexBuffer.h"
#include "Shader.h"
#include "Event.h"
#include "scene/Camera.h"
#include "scene/Light.h"
#include "scene/Model.h"
#include "scene/Vertex.h"

#include <memory>

namespace test {

class TestDrawFlat : public Test {
private:
    std::unique_ptr<Shader> shader;
    vec3 translation;

    Camera camera;

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

    std::unique_ptr<LightBlock> lights;
    vec4 defaultAmbient = { 0.2, 0.2, 0.2, 1.0 };
    vec4 defaultDirect = { 0.8, 0.8, 0.8, 1.0 };
    vec4 defaultLightPosition = { 0.0, 1.0, 1.0, 0.0 }; //infinetely far
    bool infDistLight[LightBlock::MAX_NLIGHTS] = { true };

    std::vector<Model<Vertex>> models;

    void writeQuad(int modelIdx, vec3 pos, float size, vec4 color);

public:
    TestDrawFlat();
    ~TestDrawFlat();

    void onUpdate(float deltatime) override;
    void onRender() override;
    void onImGuiRender() override;
    virtual void onEvent(Event& e) override;

    class TooManyLightsException {};
};


}

#endif // __TESTDRAWFLAT_H_
