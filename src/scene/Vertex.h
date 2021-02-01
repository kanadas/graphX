#ifndef __VERTEX_H_
#define __VERTEX_H_

#include "glad/glad.h"
#include "buffers/VertexBufferLayout.h"
#include "utils/linalg.h"
#include <memory>

class Vertex {
private:
    static const GLuint SIZE = (3 + 4) * sizeof(GLfloat);
    GLfloat* position;
    GLfloat* color;

public:
    Vertex(uint8_t* begin)
        : position((GLfloat*) begin)
        , color((GLfloat*) (begin + 3 * sizeof(GLfloat)))
    {
    }

    GLfloat* getPosition() const { return position; }
    GLfloat* getColor() const { return color; }

    static void createLayout(VertexBufferLayout& layout)
    {
        layout.push(3, GL_FLOAT); //position
        layout.push(4, GL_FLOAT); //color
    }

    static constexpr GLuint size()
    {
        return SIZE;
    }

    static std::array<uint8_t, SIZE> createData(vec3 position, vec4 color)
    {
        std::array<uint8_t, SIZE> res;
        memcpy(res.data(), position.arr, 3 * sizeof(GLfloat));
        memcpy(res.data() + 3 * sizeof(GLfloat), color.arr, 4 * sizeof(GLfloat));
        return res;
    }

    static std::array<uint8_t, SIZE> copyData(const Vertex& other)
    {
        std::array<uint8_t, SIZE> res;
        memcpy(res.data(), other.getPosition(), 3 * sizeof(GLfloat));
        memcpy(res.data() + 3 * sizeof(GLfloat), other.getColor(), 4 * sizeof(GLfloat));
        return res;
    }

};

#endif // __VERTEX_H_
