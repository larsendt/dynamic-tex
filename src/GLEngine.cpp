#include "GLEngine.h"
#include "Util.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

GLEngine::GLEngine(int argc, char** argv)
{
	fprintf(stdout, "Initializing GLEngine\n");
	m_fullScreen = false;
	m_sphere = true;
	m_godRays = true;
	m_screenWidth = 1300;
	m_screenHeight = 750;
	m_updateRate = 1.0/60.0;
	m_scale = 1.0;
	initGL(argc, argv);
	resize(m_screenWidth, m_screenHeight);
}

void GLEngine::initGL(int argc, char** argv)
{
    m_window = new sf::Window(sf::VideoMode(m_screenWidth, m_screenHeight, 32), "GLEngine");
	m_clock = new sf::Clock();
	
#ifdef USE_GLEW
    GLenum error = glewInit();
    if(GLEW_OK != error)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
    }
#endif

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	m_texShader = new Shader("shaders/warping.vert", "shaders/sphere_warping.frag");
	m_gRayShader = new Shader("shaders/god_rays.vert", "shaders/god_rays.frag");
	
	m_texFrameBuffer = new FrameBuffer(m_screenWidth, m_screenHeight);
	m_gRayFrameBuffer = new FrameBuffer(m_screenWidth, m_screenHeight);
}

int GLEngine::begin()
{
	while(m_window->IsOpened())
	{
		sf::Event Event;
		while(m_window->GetEvent(Event))
		{
			if(Event.Type == sf::Event::Closed)
			{
				m_window->Close();
				return 0;
			}
			else if(Event.Type == sf::Event::MouseButtonPressed)
			{
			    m_mouseLastX = Event.MouseButton.X;
			    m_mouseLastY = Event.MouseButton.Y;
		    }
		    else if(Event.Type == sf::Event::MouseMoved && m_window->GetInput().IsMouseButtonDown(sf::Mouse::Left))
		    {
		        int dx = Event.MouseMove.Y - m_mouseLastY; 
		        int dy = Event.MouseMove.X - m_mouseLastX;
		        m_mouseRotX += dx;
		        m_mouseRotY += dy;
		        m_mouseLastX = Event.MouseMove.X;
		        m_mouseLastY = Event.MouseMove.Y;
	        }
	        else if(Event.Type == sf::Event::MouseWheelMoved)
	        {
	            m_scale += (float)Event.MouseWheel.Delta / 10.0;
            }
			else if(Event.Type == sf::Event::KeyPressed)
			{
			    switch(Event.Key.Code)
			    {
			        case sf::Key::Escape:
			            m_window->Close();
			            return 0;
			        case sf::Key::F:
			            m_fullScreen = !m_fullScreen; 
				        //This breaks things...
                        //m_window->Create(sf::VideoMode(m_window->GetWidth(), m_window->GetHeight(), 32), "GLEngine", (m_fullScreen ? sf::Style::Fullscreen : sf::Style::Resize|sf::Style::Close)); 
                        break;
                    case sf::Key::S:
                        m_sphere = !m_sphere;
                        break;
                    case sf::Key::G:
                        m_godRays = !m_godRays;
                        break;
                    default:
                        break;
			    }
			}
			else if(Event.Type == sf::Event::Resized)
			{
				resize(Event.Size.Width, Event.Size.Height);
			}
		}
		
		update();
		drawScene();
		m_window->Display();
	}
	return 0;
}


void GLEngine::drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	m_texFrameBuffer->bind();
    m_texShader->bind();
    m_texShader->setUniform1f("screen", m_width);
    m_texShader->setUniform1f("time", m_time);
        
    glBegin(GL_POLYGON);
    glVertex3f(-m_width, -1, 0);
    glVertex3f(m_width, -1, 0);
    glVertex3f(m_width, 1, 0);
    glVertex3f(-m_width, 1, 0);
    glEnd();
    
    m_texShader->release();
    m_texFrameBuffer->release();
    m_texture = m_texFrameBuffer->texture();
    
    glBindTexture(GL_TEXTURE_2D, m_texture);	
	
	if(m_sphere)
	{
	    m_gRayFrameBuffer->bind();
	    
	    glPushMatrix();
	    //glTranslatef(0.0, 0.0, -4.0);
	    glRotatef(m_mouseRotX, 1, 0, 0);
	    glRotatef(m_mouseRotY, 0, 1, 0);
	    glScalef(m_scale, m_scale, m_scale);
	    GLUquadricObj* sphere = gluNewQuadric();
		gluQuadricDrawStyle(sphere, GLU_FILL);
		gluQuadricTexture(sphere, GL_TRUE);
		gluQuadricNormals(sphere, GLU_SMOOTH);
 		gluSphere(sphere, 0.5, 50, 50);
 		
        glBindTexture(GL_TEXTURE_2D, 0);
 		glTranslatef(1.0, -1.0, 0.0);
		gluQuadricDrawStyle(sphere, GLU_FILL);
		gluQuadricTexture(sphere, GL_FALSE);
		gluQuadricNormals(sphere, GLU_SMOOTH);
 		gluSphere(sphere, 0.05, 50, 50);
 		glPopMatrix();
 		
 		m_gRayFrameBuffer->release();
 		glBindTexture(GL_TEXTURE_2D, m_gRayFrameBuffer->texture());
 		
 		if(m_godRays) m_gRayShader->bind();
        glBegin(GL_POLYGON);
        glTexCoord2f(0, 0); glVertex3f(-m_width, -1, 0);
        glTexCoord2f(1, 0); glVertex3f(m_width, -1, 0);
        glTexCoord2f(1, 1); glVertex3f(m_width, 1, 0);
        glTexCoord2f(0, 1); glVertex3f(-m_width, 1, 0);
        glEnd();
        if(m_godRays) m_gRayShader->release();
	}
	else 
	{
        glBegin(GL_POLYGON);
        glTexCoord2f(0, 0); glVertex3f(-m_width, -1, 0);
        glTexCoord2f(1, 0); glVertex3f(m_width, -1, 0);
        glTexCoord2f(1, 1); glVertex3f(m_width, 1, 0);
        glTexCoord2f(0, 1); glVertex3f(-m_width, 1, 0);
        glEnd();
    }

	glFlush();
}


void GLEngine::update()
{
	float time = m_clock->GetElapsedTime();
    m_clock->Reset();
    float rot_speed = 0.05;
    float time_speed = 0.01;
    	
    if(time < m_updateRate)
    {
    	return;
	}
	else if(time > m_updateRate)
	{
        float multiplier = time / m_updateRate;
        m_mouseRotY += rot_speed * multiplier;
        m_time += time_speed * multiplier;
    }
    else
    {	
        m_mouseRotY += rot_speed;
        m_time += time_speed;
    }
}


void GLEngine::resize(int width, int height)
{
	m_width = (height>0) ? (GLfloat)width/height : 1;
	
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(90, m_width, 0.5, 20.0);
	glOrtho(-m_width, m_width, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	m_texFrameBuffer->resize(width, height);
	m_gRayFrameBuffer->resize(width, height);
}





