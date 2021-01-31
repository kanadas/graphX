#ifndef __VERTEXARRAY_H_
#define __VERTEXARRAY_H_

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray {
private:
    GLuint rendererID;
public:
    VertexArray();
    ~VertexArray();

    void bind() const;
    void unbind() const;

    void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
};

#endif // __VERTEXARRAY_H_
