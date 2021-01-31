#ifndef __MODEL_H_
#define __MODEL_H_

#include "glad/glad.h"
#include "Renderer.h"
#include "buffers/VertexArray.h"
#include "buffers/VertexBuffer.h"
#include "buffers/IndexBuffer.h"
#include "utils/linalg.h"
#include "utils/Log.h"

#include <vector>
#include <memory>
#include <array>

class IndexOutOfBoundsException {
};

template <class VertexT>
class Model {
public:
    Model(std::string name)
        : name(name)
        , translation(0, 0, 0)
        , rotationPerAxis(0, 0, 0)
    {
        VAO = std::make_unique<VertexArray>();
        indexBuffer = std::make_unique<IndexBuffer>(nullptr, maxIndexCount, GL_DYNAMIC_DRAW);
        vertexBuffer = std::make_unique<VertexBuffer>(nullptr, maxVertexCount * VertexT::size(), GL_DYNAMIC_DRAW);
        VertexBufferLayout layout;
        VertexT::createLayout(layout);
        VAO->addBuffer(*vertexBuffer, layout);
    }

    //Result is viable till next vertex addition / deletion
    VertexT getVertex(int idx)
    {
        if (idx >= nVertices) {
            throw new IndexOutOfBoundsException();
        }
        return VertexT(buff.data() + idx * VertexT::size());
    }

    const std::vector<GLuint>& getIndices() const { return indices; }
    vec3& getTranslation() { return translation; }
    vec3& getRotationPerAxis() { return rotationPerAxis; }
    void setTranslation(const vec3& t) { translation = t; }
    void setRotationPerAxis(const vec3& r) { rotationPerAxis = r; }
    mat4 getTransform() const
    {
        rot3 rotation = rot3(vec3(1,0,0), rotationPerAxis.arr[0]) *
            rot3(vec3(0,1,0), rotationPerAxis.arr[1]) *
            rot3(vec3(0,0,1), rotationPerAxis.arr[2]);
        return mat4::Translation(translation) * rotation.toMatrix();
    }
    const std::string& getName() const { return name; }
    uint32_t getNVertices() const { return nVertices; }

    void addVertex(std::array<uint8_t, VertexT::size()> data)
    {
        nVertices++;
        buff.insert(buff.end(), data.begin(), data.end());
    }

    void removeVertex(uint32_t idx)
    {
        nVertices--;
        buff.erase(buff.begin() + idx * VertexT::size(), buff.begin() + (idx + 1) * VertexT::size());
    }

    void addIndex(uint32_t idx)
    {
        if (idx >= nVertices) {
            throw new IndexOutOfBoundsException();
        }
        indices.push_back(idx);
    }

    //Assuming drawing triangles (not strips or fans)
    void removeTriangle(uint32_t idx)
    {
        indices.erase(indices.begin() + idx * 3, indices.begin() + (idx + 1) * 3);
    }

    void sendData()
    {
        if (nVertices <= maxVertexCount) {
            vertexBuffer->sendData(buff.data(), buff.size());
        } else {
            maxVertexCount *= 2;
            vertexBuffer->reset(buff.data(), maxVertexCount * VertexT::size(), GL_DYNAMIC_DRAW);
        }

        if (indices.size() <= maxIndexCount) {
            indexBuffer->sendData(indices.data(), indices.size());
        } else {
            maxIndexCount *= 2;
            indexBuffer->reset(indices.data(), maxIndexCount, GL_DYNAMIC_DRAW);
        }
    }

    void draw(std::unique_ptr<Shader>& shader)
    {
        Renderer renderer;
        renderer.draw(*VAO, *indexBuffer, *shader, indices.size());
    }

private:
    uint32_t nVertices = 0;
    std::vector<uint8_t> buff;
    std::vector<GLuint> indices;
    vec3 translation;
    vec3 rotationPerAxis;
    std::string name;

    uint32_t maxVertexCount = 100;
    uint32_t maxIndexCount = 200;
    std::unique_ptr<VertexArray> VAO;
    std::unique_ptr<IndexBuffer> indexBuffer;
    std::unique_ptr<VertexBuffer> vertexBuffer;
};

#endif // __MODEL_H_
