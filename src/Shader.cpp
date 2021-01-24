#include "Shader.h"

#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <string>

#include "errors.h"

Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
{
    filenames[GL_VERTEX_SHADER] = vertexShader;
    filenames[GL_FRAGMENT_SHADER] = fragmentShader;
    rendererID = createShader(vertexShader, fragmentShader);
}

Shader::~Shader()
{
    unbind();
    glDeleteProgram(rendererID);
}

GLuint Shader::compileShader(const std::string& filepath, GLenum type)
{
    std::ifstream stream(filepath);

    std::string line;
    std::stringstream ss;

    while (getline(stream, line)) {
        ss << line << '\n';
    }

    GLuint id = glCreateShader(type);
    std::string str = ss.str();
    const char* src = str.c_str();
    int len = str.length();

    //std::cout << "Read "  << len << " characters from file " << filepath << std::endl;
    //std::cout << str << std::endl;

    glShaderSource(id, 1, &src, &len);
    glCompileShader(id);

    //Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length);
        glGetShaderInfoLog(id, length, &length, message);
        std::cerr << "Failed to compile ";
        if (type == GL_VERTEX_SHADER) {
            std::cerr << "vertex ";
        } else {
            std::cerr << "fragment ";
        }
        std::cerr << "shader!\n" << message << "\n";
        throw "ShaderException";
    }
    return id;
}

GLuint Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
        GLuint program = glCreateProgram();
        GLuint vs = compileShader(vertexShader, GL_VERTEX_SHADER);
        GLuint fs = compileShader(fragmentShader, GL_FRAGMENT_SHADER);
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);
        glDeleteShader(vs);
        glDeleteShader(fs);
        return program;
}

void Shader::bind() const
{
    glUseProgram(rendererID);
}

void Shader::unbind() const
{
    glUseProgram(0);
}

void Shader::setUniform1i(const std::string& name, GLint v0)
{
    glUniform1i(getUniformLocation(name), v0);
}

void Shader::setUniform1iv(const std::string& name, GLint count, const GLint *value)
{
    glUniform1iv(getUniformLocation(name), count, value);
}

void Shader::setUniform4f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
    glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
}

void Shader::setUniformMat4f(const std::string& name, GLfloat matrix[16])
{
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, matrix);
}

GLint Shader::getUniformLocation(const std::string& name) const
{
    if (uniformLocationCache.find(name) != uniformLocationCache.end()) {
        return uniformLocationCache[name];
    }
    GLint location = glGetUniformLocation(rendererID, name.c_str());
    if(location == -1) {
        std::cerr << "Uniform '" << name << "' doesn't exist!" << std::endl;
    }
    uniformLocationCache[name] = location;
    return location;
}
