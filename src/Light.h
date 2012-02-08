#ifndef LIGHT_H
#define LIGHT_H

#include <GL/gl.h>
#include <GL/glu.h>

class Light
{
    public:
        Light();
        void enlighten();
        void setPos(float x, float y, float z);
        void displaySphere(bool display_sphere);
        bool displayingSphere() { return m_displayingSphere; }
        float* modelViewMatrix() { return m_modelView; }
   
    private:
        float m_x;
        float m_y;
        float m_z;
        float m_diffuse;
        float m_specular;
        float m_ambient;
        bool m_displayingSphere;        
        float* m_modelView;
};

#endif
