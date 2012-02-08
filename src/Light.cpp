#include "Light.h"
#include <GL/glu.h>

Light::Light()
{
    m_x = 0;
    m_y = 0;
    m_z = 0;
    
    m_ambient = 0.1;
    m_diffuse = 1.0;
    m_specular = 0.6;
    
    m_displayingSphere = true;
	m_modelView = new float[16];
}

void Light::setPos(float x, float y, float z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}

void Light::displaySphere(bool display_sphere)
{
    m_displayingSphere = display_sphere;
}

void Light::enlighten()
{
    float Ambient[] = {m_ambient, m_ambient, m_ambient, 1.0};
	float Diffuse[] = {m_diffuse, m_diffuse, m_diffuse, 1.0};
	float Specular[] = {m_specular, m_specular, m_specular, 1.0};
	float Position[] = {m_x, m_y, m_z, 1.0};

    glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT , Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE , Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Specular);
	glLightfv(GL_LIGHT0, GL_POSITION, Position);
	
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(m_x, m_y, m_z);
    glGetFloatv(GL_MODELVIEW_MATRIX, m_modelView);
    
    if(m_displayingSphere)
    {   
        GLUquadricObj* sphere = gluNewQuadric();
	    gluQuadricDrawStyle(sphere, GLU_FILL);
	    glColor3f(1.0, 1.0, 1.0);
	    gluSphere(sphere, 0.4, 50, 50);
	}
	
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
}
