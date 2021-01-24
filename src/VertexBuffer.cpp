#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, GLuint size, GLenum usage)
{
    glGenBuffers(1, &rendererID);
    glBindBuffer(GL_ARRAY_BUFFER, rendererID);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

VertexBuffer::VertexBuffer(const void* data, GLuint size)
    :VertexBuffer(data, size, GL_STATIC_DRAW)
{}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &rendererID);
}

void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, rendererID);
}

void VertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void VertexBuffer::sendData(const void* data, GLuint size, GLuint offset /*= 0)*/)
{
    bind();
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}
