#ifndef __INDEXBUFFER_H_
#define __INDEXBUFFER_H_

#include <glad/glad.h>

class IndexBuffer
{
private:
    GLuint rendererID;
    GLuint count;
public:
    IndexBuffer(const GLuint* data,  GLuint count, GLenum mode = GL_STATIC_DRAW);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;
    GLuint getCount() const;
    void sendData(const void* data, GLuint size, GLuint offset = 0);
    void reset(const GLuint* data, GLuint count, GLenum mode);
};

#endif // __INDEXBUFFER_H_
