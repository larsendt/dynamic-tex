#ifndef TEXTURE_H
#define TEXTURE_H

#include <SFML/Image.hpp>
#include <GL/gl.h>

class Image
{
    public:
        Image(char* img_path);
        GLuint handle() { return m_handle; }
        
    private:
        GLuint m_handle;
        
};

#endif
