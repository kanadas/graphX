#include <glad/glad.h>
#include <iostream>

#include "errors.h"

// Callback function for printing debug statements
void APIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length, const GLchar* msg, const void* data)
{
    std::string _source;
    std::string _type;
    std::string _severity;

    switch (source) {
    case GL_DEBUG_SOURCE_API:
        _source = "API";
        break;

    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        _source = "WINDOW SYSTEM";
        break;

    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        _source = "SHADER COMPILER";
        break;

    case GL_DEBUG_SOURCE_THIRD_PARTY:
        _source = "THIRD PARTY";
        break;

    case GL_DEBUG_SOURCE_APPLICATION:
        _source = "APPLICATION";
        break;

    case GL_DEBUG_SOURCE_OTHER:
        _source = "UNKNOWN";
        break;

    default:
        _source = "UNKNOWN";
        break;
    }

    switch (type) {
    case GL_DEBUG_TYPE_ERROR:
        _type = "ERROR";
        break;

    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        _type = "DEPRECATED BEHAVIOR";
        break;

    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        _type = "UDEFINED BEHAVIOR";
        break;

    case GL_DEBUG_TYPE_PORTABILITY:
        _type = "PORTABILITY";
        break;

    case GL_DEBUG_TYPE_PERFORMANCE:
        _type = "PERFORMANCE";
        break;

    case GL_DEBUG_TYPE_OTHER:
        _type = "OTHER";
        break;

    case GL_DEBUG_TYPE_MARKER:
        _type = "MARKER";
        break;

    default:
        _type = "UNKNOWN";
        break;
    }

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        _severity = "HIGH";
        break;

    case GL_DEBUG_SEVERITY_MEDIUM:
        _severity = "MEDIUM";
        break;

    case GL_DEBUG_SEVERITY_LOW:
        _severity = "LOW";
        break;

    case GL_DEBUG_SEVERITY_NOTIFICATION:
        _severity = "NOTIFICATION";
        break;

    default:
        _severity = "UNKNOWN";
        break;
    }

    std::cout << id << ": " << _type << " of " << _severity << " severity, raised from "
              << _source << ": " << msg << std::endl;
}

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

// gluErrorString was deprecated
const char* gluErrorString(GLenum errorCode)
{
    switch (errorCode) {
    default:
        return "unknown error code";
    case GL_NO_ERROR:
        return "no error";
    case GL_INVALID_ENUM:
        return "invalid enumerant";
    case GL_INVALID_VALUE:
        return "invalid value";
    case GL_INVALID_OPERATION:
        return "invalid operation";
#ifndef GL_VERSION_3_0
    case GL_STACK_OVERFLOW:
        return "stack overflow";
    case GL_STACK_UNDERFLOW:
        return "stack underflow";
    case GL_TABLE_TOO_LARGE:
        return "table too large";
#endif
    case GL_OUT_OF_MEMORY:
        return "out of memory";
#ifdef GL_EXT_framebuffer_object
    case GL_INVALID_FRAMEBUFFER_OPERATION_EXT:
        return "invalid framebuffer operation";
#endif
    }
};

void GLCheckError(const char* msg)
{
    GLenum err;
    if((err = glGetError()) != GL_NO_ERROR) {
        fprintf(stderr, "Error %d: %s, %s\n", err, gluErrorString(err), msg);
        while ((err = glGetError()) != GL_NO_ERROR) {
            fprintf(stderr, "Error %d: %s, %s\n", err, gluErrorString(err), msg);
        }
        exit(1);
    }
}
