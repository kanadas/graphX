#include "VertexBufferLayout.h"

GLuint VertexBufferElement::getSizeOfType(GLuint type)
{
    switch (type) {
    case GL_FLOAT:
        return sizeof(GLfloat);
    case GL_INT:
        return sizeof(GLint);
    case GL_UNSIGNED_INT:
        return sizeof(GLuint);
    case GL_UNSIGNED_BYTE:
        return sizeof(GLubyte);
    default:
        throw "Unsupported attribute type";
    }
}

VertexBufferLayout::VertexBufferLayout()
    : stride(0)
{
}

void VertexBufferLayout::push(GLuint count, GLenum type)
{
    elements.push_back({ count, type, GL_FALSE });
    stride += count * VertexBufferElement::getSizeOfType(type);
}

void VertexBufferLayout::push(GLuint count, GLenum type, GLboolean normalized)
{
    elements.push_back({ count, type, normalized });
    stride += count * VertexBufferElement::getSizeOfType(type);
}

std::vector<VertexBufferElement> VertexBufferLayout::getElements() const
{
    return elements;
}

GLuint VertexBufferLayout::getStride() const
{
    return stride;
}
