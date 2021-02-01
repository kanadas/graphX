#include "Renderer.h"

void Renderer::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, GLuint indexCount) const
{
    shader.bind();
    va.bind();
    ib.bind();
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, NULL);
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    draw(va, ib, shader, ib.getCount());
}

void Renderer::drawPoints(const VertexArray& va, const Shader& shader, GLuint count) const
{
    shader.bind();
    va.bind();
    glPointSize(10.0);
    glDrawArrays(GL_POINTS, 0, count);
}

void Renderer::drawLines(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, GLuint count) const
{
    shader.bind();
    va.bind();
    ib.bind();
    glDrawElements(GL_LINES, count, GL_UNSIGNED_INT, NULL);
}
