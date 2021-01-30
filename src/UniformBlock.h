#ifndef __UNIFORMBLOCK_H_
#define __UNIFORMBLOCK_H_

#include <glad/glad.h>
#include <string>
#include <vector>

class UniformStruct
{
public:
    virtual std::vector<std::string> getNames() const = 0;
    virtual void set() const = 0;
    virtual void setOffsets(std::vector<GLint>::iterator& offsets) = 0;

protected:
    //Assuming bound buffer
    void setData(const void* data, GLuint size, GLint offset) const;
};

class UniformBlock: public UniformStruct
{
public:
    void attach(GLuint bindingPoint);
    void attach(const std::string& name, GLuint bindingPoint);
    void attach(GLuint program, const std::string& name, GLuint bindingPoint);

    GLuint getBindingPoint() { return bindingPoint; }
    GLint getBlockSize() { return blockSize; }
    GLuint getProgram() { return program; }
    std::string getName() { return name; }

    void bind() const;
    void unbind() const;

protected:
    GLuint rendererID = 0;
    GLuint bindingPoint = 0;
    GLint blockSize = 0;
    GLuint program = 0;
    std::string name;

    void createUniformBlock(const std::string name, GLuint prog, GLuint bindingPoint);
    void createUniformBlock(const std::string name, GLuint prog);
    void removeBlock();

    static GLint maxBindingPoint;
    static GLint nextBindingPoint;

    GLuint newUniformBindingPoint();
};

class UniformBindingPointsLimitExceededException {};
class InvalidUniformBlockIndexException {};

#endif // __UNIFORMBLOCK_H_
