#ifndef __VERTEXBUFFER_H_
#define __VERTEXBUFFER_H_

#include "glad/glad.h"

class VertexBuffer {
private:
    GLuint rendererID;

public:
    VertexBuffer(const void* data, GLuint size, GLenum usage);
    VertexBuffer(const void* data, GLuint size);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

    void sendData(const void* data, GLuint size, GLuint offset = 0);
};

#endif // __VERTEXBUFFER_H_
