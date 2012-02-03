#ifndef TEXTURE_H
#define TEXTURE_H

#include <SFML/Graphics.hpp>

class Texture
{
    public:
        static GLuint loadTexture(const char* img_path);
        
    private:
        Texture();
};

#endif
