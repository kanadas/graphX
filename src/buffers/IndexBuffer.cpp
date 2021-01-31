#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const GLuint* data, GLuint count, GLenum mode)
    : count(count)
{
    glGenBuffers(1, &rendererID);
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, mode);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &rendererID);
}

void IndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
}

void IndexBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLuint IndexBuffer::getCount() const
{
    return count;
}

void IndexBuffer::sendData(const void* data, GLuint size, GLuint offset /*= 0)*/)
{
    bind();
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset * sizeof(GLuint), size * sizeof(GLuint), data);
}

void IndexBuffer::reset(const GLuint* data, GLuint count, GLenum mode)
{
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, mode);
}
