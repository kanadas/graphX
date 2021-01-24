#ifndef __SHADER_H_
#define __SHADER_H_

#include <string>
#include <unordered_map>
#include <glad/glad.h>

class Shader
{
private:
    GLuint rendererID;
    std::unordered_map<GLenum, std::string> filenames;
    mutable std::unordered_map<std::string, int> uniformLocationCache;

    GLuint compileShader(const std::string& filepath, GLenum type);
    GLuint createShader(const std::string& vertexShader, const std::string& fragmentShader);
public:
    Shader(const std::string& vertexShader, const std::string& fragmentShader);
    ~Shader();

    void bind() const;
    void unbind() const;

    void setUniform1i(const std::string& name, GLint v0);
    void setUniform1iv(const std::string& name, GLint count, const GLint *value);
    void setUniform4f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
    void setUniformMat4f(const std::string& name, GLfloat matrix[16]);

    GLint getUniformLocation(const std::string& name) const;
};

#endif // __SHADER_H_
