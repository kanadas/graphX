#ifndef __RENDERER_H_
#define __RENDERER_H_

#include <glad/glad.h>

#include "buffers/VertexArray.h"
#include "buffers/IndexBuffer.h"
#include "Shader.h"

class Renderer
{

public:
    void clear() const;
    void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, GLuint count) const;
};

#endif // __RENDERER_H_
