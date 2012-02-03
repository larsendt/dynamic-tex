#include "Texture.h"
#include <stdio.h>
#include <stdlib.h>

Texture::Texture()
{

}

GLuint Texture::loadTexture(const char* img_path)
{
    sf::Image* img = new sf::Image();
    if(img->LoadFromFile(img_path))
    {
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->GetWidth(), img->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img->GetPixelsPtr());
	    glBindTexture(GL_TEXTURE_2D, 0);
	    return texture;
	    
    }
    else
    {
        printf("Failed to load texture: %s\n", img_path);
        exit(1);
    }
    
    return 0;
}
