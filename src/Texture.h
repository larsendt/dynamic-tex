#ifndef TEXTURE_H
#define TEXTURE_H

#include <SFML/Graphics.hpp>
#include <GL/gl.h>

class Texture
{
    public:
        Texture(char* img_path);
        GLuint handle() { return m_handle; }
        
    private:
        GLuint m_handle;
        
};

#endif
