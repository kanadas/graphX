#ifndef __VERTEXBUFFERLAYOUT_H_
#define __VERTEXBUFFERLAYOUT_H_

#include "glad/glad.h"
#include <vector>

struct VertexBufferElement {
    GLuint count;
    GLuint type;
    GLboolean normalized;

    static GLuint getSizeOfType(GLuint type);
};

class VertexBufferLayout {
private:
    std::vector<VertexBufferElement> elements;
    GLuint stride;

public:
    VertexBufferLayout();

    void push(GLuint count, GLenum type);

    void push(GLuint count, GLenum type, GLboolean normalized);

    std::vector<VertexBufferElement> getElements() const;

    GLuint getStride() const;
};

#endif // __VERTEXBUFFERLAYOUT_H_
