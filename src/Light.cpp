#include "Light.h"

Light::Light()
{
    m_x = 0;
    m_y = 0;
    m_z = 0;
    
    m_ambient = 1.0;
    m_diffuse = 1.0;
    m_specular = 1.0;
}

void Light::enlighten()
{
    float Ambient[] = {m_ambient, m_ambient, m_ambient, 1.0};
	float Diffuse[] = {m_diffuse, m_diffuse, m_diffuse, 1.0};
	float Specular[] = {m_specular, m_specular, m_specular, 1.0};
	float Position[] = {m_x, m_y, m_z, 0.0};

	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_AMBIENT , Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE , Diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Specular);
	glLightfv(GL_LIGHT0, GL_POSITION, Position);
}
