#ifndef __INDEXBUFFER_H_
#define __INDEXBUFFER_H_

#include <glad/glad.h>

class IndexBuffer
{
private:
    GLuint rendererID;
    GLuint count;
public:
    IndexBuffer(const GLuint* data,  GLuint count);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;
    GLuint getCount() const;
};

#endif // __INDEXBUFFER_H_
