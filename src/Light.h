#ifndef LIGHT_H
#define LIGHT_H

#include <GL/gl.h>
#include <GL/glu.h>

class Light
{
    public:
        Light();
        void enlighten();
   
    private:
        float m_x;
        float m_y;
        float m_z;
        float m_diffuse;
        float m_specular;
        float m_ambient;
};

#endif
