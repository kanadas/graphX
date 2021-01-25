#ifndef __ERRORS_H_
#define __ERRORS_H_

#include <glad/glad.h>
#include <string>

void APIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id,
                                     GLenum severity, GLsizei length,
                                     const GLchar *msg, const void *data);

const char *gluErrorString(GLenum errorCode);

void GLClearError();

void GLCheckError(const char *msg);

#endif // __UTILS_H_
