#ifndef __TEXTURE_H_
#define __TEXTURE_H_

#include "Renderer.h"

class Texture
{
private:
    GLuint rendererID;
    std::string filePath;
    GLubyte* localBuffer;
    GLint width, height, BPP;

public:
    Texture(const std::string& path);
    ~Texture();

    void bind(GLuint slot = 0) const;
    void unbind() const;

    inline int getWidth() const { return width; }
    inline int getHeight() const { return height; }
};

#endif // __TEXTURE_H_
