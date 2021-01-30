#include "Shader.h"

#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <string>
#include "utils/Log.h"

Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
{
    filenames[GL_VERTEX_SHADER] = vertexShader;
    filenames[GL_FRAGMENT_SHADER] = fragmentShader;
    rendererID = createShader(vertexShader, fragmentShader);
}

Shader::Shader(const std::string& vertexShader, const std::string& geomShader, const std::string& fragmentShader)
{
    filenames[GL_VERTEX_SHADER] = vertexShader;
    filenames[GL_GEOMETRY_SHADER] = geomShader;
    filenames[GL_FRAGMENT_SHADER] = fragmentShader;
    rendererID = createShader(vertexShader, geomShader, fragmentShader);
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
        switch (type) {
        case GL_VERTEX_SHADER:
            std::cerr << "vertex ";
            break;
        case GL_GEOMETRY_SHADER:
            std::cerr << "geometry ";
            break;
        case GL_FRAGMENT_SHADER:
            std::cerr << "fragment ";
            break;
        }
        std::cerr << "shader!\n"
                  << message << "\n";
        throw "ShaderException";
    }
    TRACE("Compiled shader {}", filepath);
    return id;
}

GLuint Shader::createShaders(const std::vector<std::pair<std::string, GLenum>>& shaders)
{
    GLuint program = glCreateProgram();
    for (auto it = shaders.begin(); it != shaders.end(); it++) {
        GLuint shader = compileShader(it->first, it->second);
        glAttachShader(program, shader);
        glDeleteShader(shader);
    }
    glLinkProgram(program);
    glValidateProgram(program);

#ifdef SPDLOG_DEBUG_ON
    //Print all uniforms
    GLint count;
    const GLsizei bufSize = 32; // maximum name length
    GLchar name[bufSize]; // variable name in GLSL
    GLsizei length;
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
    printf("Active Uniforms: %d\n", count);
    for (int i = 0; i < count; i++) {
        GLint size; // size of the variable
        GLenum type; // type of the variable (float, vec3 or mat4, etc)
        glGetActiveUniform(program, (GLuint)i, bufSize, &length, &size, &type, name);
        DEBUG("Uniform #{} Type: {} Name: {}", i, type, name);
    }
#endif //SPDLOG_DEBUG_ON

    return program;
}

GLuint Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    std::vector<std::pair<std::string, GLenum>> shaders = { { vertexShader, GL_VERTEX_SHADER },
        { fragmentShader, GL_FRAGMENT_SHADER } };
    return createShaders(shaders);
}

GLuint Shader::createShader(const std::string& vertexShader, const std::string& geomShader, const std::string& fragmentShader)
{
    std::vector<std::pair<std::string, GLenum>> shaders = { { vertexShader, GL_VERTEX_SHADER },
        { geomShader, GL_GEOMETRY_SHADER },
        { fragmentShader, GL_FRAGMENT_SHADER } };
    return createShaders(shaders);
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

void Shader::setUniform1iv(const std::string& name, GLint count, const GLint* value)
{
    glUniform1iv(getUniformLocation(name), count, value);
}

void Shader::setUniform3f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2)
{
    glUniform3f(getUniformLocation(name), v0, v1, v2);
}

void Shader::setUniform3f(const std::string& name, const vec3& v)
{
    setUniform3f(name, v.arr[0], v.arr[1], v.arr[2]);
}

void Shader::setUniform4f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
    glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
}

void Shader::setUniform4f(const std::string& name, const vec4& v)
{
    setUniform4f(name, v.arr[0], v.arr[1], v.arr[2], v.arr[3]);
}

void Shader::setUniformMat4f(const std::string& name, const GLfloat matrix[16])
{
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, matrix);
}

void Shader::setUniformMat4f(const std::string& name, const mat4& m)
{
    setUniformMat4f(name, m.arr);
}

GLint Shader::getUniformLocation(const std::string& name) const
{
    if (uniformLocationCache.find(name) != uniformLocationCache.end()) {
        return uniformLocationCache[name];
    }
    GLint location = glGetUniformLocation(rendererID, name.c_str());
    if (location == -1) {
        std::cerr << "Uniform '" << name << "' doesn't exist!" << std::endl;
    }
    uniformLocationCache[name] = location;
    return location;
}
