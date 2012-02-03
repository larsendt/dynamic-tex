#include "GLEngine.h"
#include "Util.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUM_COLORS 3

GLEngine::GLEngine(int argc, char** argv)
{
	fprintf(stdout, "Initializing GLEngine\n");
	initGL(argc, argv);
}

void GLEngine::initGL(int argc, char** argv)
{
    m_window = new sf::Window(sf::VideoMode::GetDesktopMode(), "GLEngine", sf::Style::Fullscreen);
    //m_window = new sf::Window(sf::VideoMode(1300, 700), "GLEngine", sf::Style::Resize | sf::Style::Close); 
    m_screenWidth = m_window->GetWidth();
    m_screenHeight = m_window->GetHeight();
	m_clock = new sf::Clock();
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHT0);
    
    m_fullScreen = false;
	m_sphere = false;
	m_godRays = false;
	m_shaderIndex = 0;
	m_wrapTex = false;
	m_planet = false;
	m_equiWarp = false;
	m_updateRate = 1.0/60.0;
	m_scale = 1.0;

    m_light = new Light();

	m_gRayShader = new Shader("shaders/god_rays.vert", "shaders/god_rays.frag");
	m_texWrappingShader = new Shader("shaders/wrap_texture.vert", "shaders/wrap_texture.frag");
	m_equiWarpingShader = new Shader("shaders/equirectangular_warping.vert", "shaders/equirectangular_warping.frag");
	
	m_colorShaders.push_back(new Shader("shaders/warping.vert", "shaders/warping_nebulous.frag"));
	m_colorShaders.push_back(new Shader("shaders/warping.vert", "shaders/warping_2pass_gold.frag"));
	m_colorShaders.push_back(new Shader("shaders/warping.vert", "shaders/warping_chaotic_red.frag"));
	m_currentColorShader = m_colorShaders[0];
	
	m_texFrameBuffer = new FrameBuffer(m_screenWidth, m_screenHeight);
	m_gRayFrameBuffer = new FrameBuffer(m_screenWidth, m_screenHeight);
	m_texWrappingFrameBuffer = new FrameBuffer(m_screenWidth, m_screenHeight);
	m_equiWarpingFrameBuffer = new FrameBuffer(m_screenWidth, m_screenHeight);
	
	m_mouseRotX = 0;
	m_mouseRotY = 0;
	m_mouseLastX = 0;
	m_mouseLastY = 0;
	
	resize(m_screenWidth, m_screenHeight);
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
                    case sf::Key::C:
                        m_shaderIndex += 1;
                        if(m_shaderIndex > NUM_COLORS-1)
                        {
                            m_shaderIndex = 0;
                        }
                        m_currentColorShader = m_colorShaders[m_shaderIndex];
                        break;
                    case sf::Key::W:
                        m_wrapTex = !m_wrapTex;
                        break;
                    case sf::Key::P:
                        m_planet = !m_planet;
                        break;
                    case sf::Key::E:
                        m_equiWarp = !m_equiWarp;
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
	
    m_currentColorShader->bind();
    m_currentColorShader->setUniform1f("screen", m_width);
    m_currentColorShader->setUniform1f("time", m_time);
    
    glBegin(GL_POLYGON);
    glVertex3f(-m_width, -1, 0);
    glVertex3f(m_width, -1, 0);
    glVertex3f(m_width, 1, 0);
    glVertex3f(-m_width, 1, 0);
    glEnd();
    
    m_currentColorShader->release();
    m_texFrameBuffer->release();
    m_texture = m_texFrameBuffer->texture();
    
    if(m_equiWarp)
    {
        m_equiWarpingFrameBuffer->bind();
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        m_equiWarpingShader->bind();
        m_equiWarpingShader->setUniform1i("texture", 0);
        m_equiWarpingShader->setUniform1f("screen", m_width);
        
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_POLYGON);
        glTexCoord2f(0, 0); glVertex3f(-m_width, -1, 0);
        glTexCoord2f(1, 0); glVertex3f(m_width, -1, 0);
        glTexCoord2f(1, 1); glVertex3f(m_width, 1, 0);
        glTexCoord2f(0, 1); glVertex3f(-m_width, 1, 0);
        glEnd();
        
        m_equiWarpingShader->release();
        m_equiWarpingFrameBuffer->release();
        m_texture = m_equiWarpingFrameBuffer->texture();
    }
    
    if(m_wrapTex)
    {
        m_texWrappingFrameBuffer->bind();
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        m_texWrappingShader->bind();
        m_texWrappingShader->setUniform1i("texture", 0);
        
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_POLYGON);
        glTexCoord2f(0, 0); glVertex3f(-m_width, -1, 0);
        glTexCoord2f(1, 0); glVertex3f(m_width, -1, 0);
        glTexCoord2f(1, 1); glVertex3f(m_width, 1, 0);
        glTexCoord2f(0, 1); glVertex3f(-m_width, 1, 0);
        glEnd();
        
        m_texWrappingShader->release();
        m_texWrappingFrameBuffer->release();
        m_texture = m_texWrappingFrameBuffer->texture();
    }
    
    glBindTexture(GL_TEXTURE_2D, m_texture);	
        
	if(m_sphere)
	{
	    m_gRayFrameBuffer->bind();
	    
	    glPushMatrix();
	    
	    //glTranslatef(0.0, 0.0, -4.0);
	    
	    glDisable(GL_LIGHTING);
	    
	    glRotatef(m_mouseRotX, 1, 0, 0);
	    glRotatef(m_mouseRotY, 0, 1, 0);
	    glScalef(m_scale, m_scale, m_scale);
	    GLUquadricObj* sphere = gluNewQuadric();
		gluQuadricDrawStyle(sphere, GLU_FILL);
		gluQuadricTexture(sphere, GL_TRUE);
		gluQuadricNormals(sphere, GLU_SMOOTH);
		glColor3f(1.0, 1.0, 1.0);
 		gluSphere(sphere, 0.5, 50, 50);
 		
 		glEnable(GL_LIGHTING);
 		
 		if(m_planet)
 		{
	        m_light->enlighten();
            glBindTexture(GL_TEXTURE_2D, 0);
     		glTranslatef(0.8, 0.0, 0.0);
		    gluQuadricDrawStyle(sphere, GLU_FILL);
		    gluQuadricTexture(sphere, GL_FALSE);
		    gluQuadricNormals(sphere, GLU_SMOOTH);
		    glColor3f(0.2, 0.2, 0.2);
     		gluSphere(sphere, 0.05, 50, 50);
 		}
 		
 		glPopMatrix();
     		
 		m_gRayFrameBuffer->release();
 		
 		
	    glDisable(GL_LIGHTING);
 		glBindTexture(GL_TEXTURE_2D, m_gRayFrameBuffer->texture());
 		
 		if(m_godRays) m_gRayShader->bind();
 		glColor3f(1.0, 1.0, 1.0);
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
	    glDisable(GL_LIGHTING);
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
	//gluPerspective(45, m_width, 0.5, 20.0);
	glOrtho(-m_width, m_width, -1.0, 1.0, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	m_texFrameBuffer->resize(width, height);
	m_gRayFrameBuffer->resize(width, height);
	m_texWrappingFrameBuffer->resize(width, height);
	m_equiWarpingFrameBuffer->resize(width, height);
}





