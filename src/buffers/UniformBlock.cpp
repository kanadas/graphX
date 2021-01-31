#include "UniformBlock.h"

#include "utils/Log.h"

GLint UniformBlock::maxBindingPoint = 0;
GLint UniformBlock::nextBindingPoint = 0;

void UniformBlock::createUniformBlock(const std::string name, GLuint prog, GLuint bindingPoint)
{
    TRACE("Creating uniform block {}", name);
    std::vector<std::string> names = getNames();
    GLuint index = glGetUniformBlockIndex(prog, name.c_str());
    if (index == GL_INVALID_INDEX) {
        throw new InvalidUniformBlockIndexException();
    }
    glGetActiveUniformBlockiv(prog, index, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
    if (names.size() > 0) {
        GLuint indices[names.size()];
        char* cstrings[names.size()];
        std::vector<GLint> offsets;
        offsets.reserve(names.size());
        for (int i = 0; i < names.size(); i++) {
            cstrings[i] = const_cast<char*>(names[i].c_str());
        }
        glGetUniformIndices(prog, names.size(), cstrings, indices);
        glGetActiveUniformsiv(prog, names.size(), indices, GL_UNIFORM_OFFSET, offsets.data());
        auto off = offsets.begin();
        setOffsets(off);
    }
    glUniformBlockBinding(prog, index, bindingPoint);

    glGenBuffers(1, &rendererID);
    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, rendererID);
    glBufferData(GL_UNIFORM_BUFFER, blockSize, NULL, GL_DYNAMIC_DRAW);
}

void UniformBlock::createUniformBlock(const std::string name, GLuint prog)
{
    createUniformBlock(name, prog, newUniformBindingPoint());
}

void UniformBlock::removeBlock()
{
    if(rendererID != 0) {
        glDeleteBuffers(1, &rendererID);
    }
}

GLuint UniformBlock::newUniformBindingPoint()
{
    if (maxBindingPoint == 0) {
        glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxBindingPoint);
    }
    if (nextBindingPoint < maxBindingPoint) {
        return nextBindingPoint++;
    } else {
        throw new UniformBindingPointsLimitExceededException();
    }
}

void UniformBlock::attach(GLuint bindingPoint)
{
    attach(program, name, bindingPoint);
}

void UniformBlock::attach(const std::string& name, GLuint bindingPoint)
{
    attach(program, name, bindingPoint);
}

void UniformBlock::attach(GLuint program, const std::string& name, GLuint bindingPoint)
{
    this->program = program;
    this->name = name;
    GLuint ind;
    ind = glGetUniformBlockIndex(program, name.c_str());
    glUniformBlockBinding(program, ind, bindingPoint);
}

void UniformBlock::bind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, rendererID);
}

void UniformBlock::unbind() const
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformStruct::setData(const void* data, GLuint size, GLint offset) const
{
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}
