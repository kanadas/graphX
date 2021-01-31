#include "VertexArray.h"

#include "VertexBufferLayout.h"
#include <glad/glad.h>

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &rendererID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &rendererID);
}

void VertexArray::bind() const
{
    glBindVertexArray(rendererID);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    bind();
    vb.bind();
    const auto& elements = layout.getElements();
    unsigned long long offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (GLvoid*) offset);
        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
    }
}
