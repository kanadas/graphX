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
    Model(std::string name, uint32_t maxVertexCount, uint32_t maxIndexCount)
        : name(name)
        , translation(0, 0, 0)
        , rotationPerAxis(0, 0, 0)
        , maxVertexCount(maxVertexCount)
        , maxIndexCount(maxIndexCount)
    {
        VAO = std::make_unique<VertexArray>();
        if (maxIndexCount > 0) {
            indexBuffer = std::make_unique<IndexBuffer>(nullptr, maxIndexCount, GL_DYNAMIC_DRAW);
        }
        vertexBuffer = std::make_unique<VertexBuffer>(nullptr, maxVertexCount * VertexT::size(), GL_DYNAMIC_DRAW);
        VertexBufferLayout layout;
        VertexT::createLayout(layout);
        VAO->addBuffer(*vertexBuffer, layout);
    }

    Model(std::string name)
        : Model(name, 20, 50)
    {
    }

    //Result is viable till next vertex addition / deletion
    VertexT getVertex(int idx)
    {
        if (idx >= nVertices) {
            DEBUG("Got idx = {} but there are {} vertices", idx, nVertices);
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
        rot3 rotation = rot3(vec3(1, 0, 0), rotationPerAxis.arr[0]) * rot3(vec3(0, 1, 0), rotationPerAxis.arr[1]) * rot3(vec3(0, 0, 1), rotationPerAxis.arr[2]);
        return mat4::Translation(translation) * rotation.toMatrix();
    }
    const std::string& getName() const { return name; }
    uint32_t getNVertices() const { return nVertices; }

    void addVertex(std::array<uint8_t, VertexT::size()> data)
    {
        nVertices++;
        buff.insert(buff.end(), data.begin(), data.end());
    }

    //removes vertex idx and all its triangles (assuming triangles)
    virtual void removeVertex(uint32_t idx)
    {
        TRACE("Removing vertex {}", idx);
        nVertices--;
        std::vector<int> toRemove;
        for (int i = 0; i < indices.size(); i++) {
            if (indices[i] == idx) {
                toRemove.push_back(i / 3 - toRemove.size());
            } else if (indices[i] > idx) {
                indices[i]--;
            }
        }
        for (int idx : toRemove) {
            removeTriangle(idx);
        }
        buff.erase(buff.begin() + idx * VertexT::size(), buff.begin() + (idx + 1) * VertexT::size());
        TRACE("Removed vertex {}", idx);
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

        if (maxIndexCount > 0) {
            if (indices.size() <= maxIndexCount) {
                indexBuffer->sendData(indices.data(), indices.size());
            } else {
                maxIndexCount *= 2;
                indexBuffer->reset(indices.data(), maxIndexCount, GL_DYNAMIC_DRAW);
            }
        }
    }

    virtual void draw(std::unique_ptr<Shader>& shader)
    {
        Renderer renderer;
        if (nVertices >= 3 && indices.size() >= 3) {
            renderer.draw(*VAO, *indexBuffer, *shader, indices.size());
        }
    }

protected:
    uint32_t nVertices = 0;
    std::vector<uint8_t> buff;
    std::vector<GLuint> indices;
    vec3 translation;
    vec3 rotationPerAxis;
    std::string name;

    uint32_t maxVertexCount;
    uint32_t maxIndexCount;
    std::unique_ptr<VertexArray> VAO;
    std::unique_ptr<IndexBuffer> indexBuffer;
    std::unique_ptr<VertexBuffer> vertexBuffer;
};

template <class VertexT>
class PointModel : public Model<VertexT> {
public:
    PointModel(std::string name)
        : Model<VertexT>(name, 20, 0)
    {
    }
    PointModel(std::string name, uint32_t maxVertexCount)
        : Model<VertexT>(name, maxVertexCount, 0)
    {
    }

    virtual void draw(std::unique_ptr<Shader>& shader)
    {
        Renderer renderer;
        if (this->nVertices >= 1) {
            renderer.drawPoints(*this->VAO, *shader, this->nVertices);
        }
    }

    virtual void removeVertex(uint32_t idx)
    {
        this->nVertices--;
        this->buff.erase(this->buff.begin() + idx * VertexT::size(), this->buff.begin() + (idx + 1) * VertexT::size());
    }
};

template <class VertexT>
class LineModel : public Model<VertexT> {
public:
    LineModel(std::string name)
        : Model<VertexT>(name)
    {
    }

    LineModel(std::string name, uint32_t maxVertexCount, uint32_t maxIndexCount)
        : Model<VertexT>(name, maxVertexCount, maxIndexCount)
    {
    }

    virtual void draw(std::unique_ptr<Shader>& shader)
    {
        Renderer renderer;
        if (this->nVertices >= 2 && this->indices.size() >= 2) {
            renderer.drawLines(*this->VAO, *this->indexBuffer, *shader, this->indices.size());
        }
    }

    void removeLine(uint32_t idx)
    {
        this->indices.erase(this->indices.begin() + idx * 2, this->indices.begin() + (idx + 1) * 2);
    }

    //removes vertex idx and all its lines (assuming lines)
    virtual void removeVertex(uint32_t idx)
    {
        this->nVertices--;
        std::vector<int> toRemove;
        for (int i = 0; i < this->indices.size(); i++) {
            if (this->indices[i] == idx) {
                toRemove.push_back(i / 2 - toRemove.size());
            } else if (this->indices[i] > idx) {
                this->indices[i]--;
            }
        }
        for (int idx : toRemove) {
            removeLine(idx);
        }
        this->buff.erase(this->buff.begin() + idx * VertexT::size(), this->buff.begin() + (idx + 1) * VertexT::size());
    }
};

#endif // __MODEL_H_
