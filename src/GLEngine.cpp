#include "GLEngine.h"
#include "Util.h"
#include "Texture.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FRAMEBUFFER_DIM 1024

GLEngine::GLEngine(int argc, char** argv)
{
    fprintf(stdout, "Initializing GLEngine\n");
    initGL(argc, argv);
}

void GLEngine::initGL(int argc, char** argv)
{
    //m_window = new sf::Window(sf::VideoMode::GetDesktopMode(), "GLEngine", sf::Style::Fullscreen);
    m_window = new sf::Window(sf::VideoMode(1300, 700), "GLEngine", sf::Style::Resize | sf::Style::Close); 
    m_screenWidth = m_window->getSize().x;
    m_screenHeight = m_window->getSize().y;
    m_clock = new sf::Clock();
    
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glEnable(GL_COLOR_MATERIAL);
    
    m_fullScreen = true;
    m_sphere = true;
    m_godRays = true;
    m_shaderIndex = 0;
    m_wrapTex = true;
    m_planet = true;
    m_equiWarp = true;
    m_warpingEarth = false;
    m_updateRate = 1.0/60.0;
    m_scale = 1.0;

    m_light = new Light();

    m_gRayShader = new Shader("shaders/god_rays.vert", "shaders/god_rays.frag");
    m_texWrappingShader = new Shader("shaders/wrap_texture.vert", "shaders/wrap_texture.frag");
    m_equiWarpingShader = new Shader("shaders/equirectangular_warping.vert", "shaders/equirectangular_warping.frag");
    
    m_colorShaders.push_back(new Shader("shaders/warping.vert", "shaders/darkblue.frag"));
    m_colorShaders.push_back(new Shader("shaders/warping.vert", "shaders/chaotic1.frag"));
    m_colorShaders.push_back(new Shader("shaders/warping.vert", "shaders/chaotic2.frag"));
    m_colorShaders.push_back(new Shader("shaders/warping.vert", "shaders/chaotic3.frag"));
    m_colorShaders.push_back(new Shader("shaders/warping.vert", "shaders/warping_nebulous.frag"));
    m_colorShaders.push_back(new Shader("shaders/warping.vert", "shaders/warping_2pass_gold_soft.frag"));
    m_colorShaders.push_back(new Shader("shaders/warping.vert", "shaders/warping_2pass_gold.frag"));
    m_currentColorShader = m_colorShaders[0];
    
    m_texFrameBuffer = new FrameBuffer(m_screenWidth, m_screenHeight);
    m_gRayFrameBuffer = new FrameBuffer(m_screenWidth, m_screenHeight);
    m_texWrappingFrameBuffer = new FrameBuffer(m_screenWidth, m_screenHeight);
    m_equiWarpingFrameBuffer = new FrameBuffer(m_screenWidth, m_screenHeight);

    m_mouseRotX = 2;
    m_mouseRotY = -120;
    m_mouseLastX = 0;
    m_mouseLastY = 0;
    
    resize(m_screenWidth, m_screenHeight);
}

int GLEngine::begin()
{
    while(m_window->isOpen())
    {
        sf::Event event;
        while(m_window->waitEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                m_window->close();
                return 0;
            }
            else if(event.type == sf::Event::MouseButtonPressed)
            {
                m_mouseLastX = event.mouseButton.x;
                m_mouseLastY = event.mouseButton.y;
            }
            else if(event.type == sf::Event::MouseMoved && sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                int dx = event.mouseMove.y - m_mouseLastY; 
                int dy = event.mouseMove.x - m_mouseLastX;
                m_mouseRotX += dx;
                m_mouseRotY += dy;
                m_mouseLastX = event.mouseMove.x;
                m_mouseLastY = event.mouseMove.y;
            }
            else if(event.type == sf::Event::MouseWheelMoved)
            {
                m_scale += (float)event.mouseWheel.delta / 10.0;
            }
            else if(event.type == sf::Event::KeyPressed)
            {
                switch(event.key.code)
                {
                    case sf::Keyboard::Escape:
                        m_window->close();
                        return 0;
                    case sf::Keyboard::F:
                        // this is broken, not sure why
                        /*m_fullScreen = !m_fullScreen;
                        m_window->close(); 
                        if(m_fullScreen) 
                        {
                            m_window->Create(sf::VideoMode::GetDesktopMode(), "GLEngine", sf::Style::Fullscreen);
                        }
                        else 
                        {
                            m_window->Create(sf::VideoMode(800, 600), "GLEngine", sf::Style::Resize | sf::Style::Close); 
                        }
                        
                        m_screenWidth = m_window->GetWidth();
                        m_screenHeight = m_window->GetHeight();
                        resize(m_screenWidth, m_screenHeight);*/                   
                        break;
                    case sf::Keyboard::S:
                        m_sphere = !m_sphere;
                        break;
                    case sf::Keyboard::G:
                        m_godRays = !m_godRays;
                        break;
                    case sf::Keyboard::C:
                        m_shaderIndex += 1;
                        if(m_shaderIndex > m_colorShaders.size()-1)
                        {
                            m_shaderIndex = 0;
                        }
                        m_currentColorShader = m_colorShaders[m_shaderIndex];
                        break;
                    case sf::Keyboard::X:
                        m_shaderIndex -= 1;
                        if(m_shaderIndex < 0)
                        {
                            m_shaderIndex = m_colorShaders.size()-1;
                        }
                        m_currentColorShader = m_colorShaders[m_shaderIndex];
                        break;
                    case sf::Keyboard::W:
                        m_wrapTex = !m_wrapTex;
                        break;
                    case sf::Keyboard::P:
                        m_planet = !m_planet;
                        break;
                    case sf::Keyboard::E:
                        m_equiWarp = !m_equiWarp;
                        break;
                    case sf::Keyboard::L:
                        m_light->displaySphere(!m_light->displayingSphere());
                        break;
                    default:
                        break;
                }
            }
            else if(event.type == sf::Event::Resized)
            {
                resize(event.size.width, event.size.height);
            }
        }
        
        update();
        drawScene();
        m_window->display();
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
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);    
        
    if(m_sphere)
    {
        m_gRayFrameBuffer->bind();
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45, m_width, 0.05, 20.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        gluLookAt(0, 0, 2, 0, 0, 0, 0, 1, 0);        
        
        glPushMatrix();
        glRotatef(m_mouseRotX, 1, 0, 0);
        glRotatef(m_mouseRotY, 0, 1, 0);
        glScalef(m_scale, m_scale, m_scale);
        
        if(m_godRays)
        {
            m_light->setPos(0, 0, 0);
        }
        else
        {
            m_light->setPos(5, 0, 0);
        }
        
        m_light->enlighten();
        
        if(m_godRays)
        {
            glDisable(GL_LIGHTING);
        }
        
        GLUquadricObj* sphere = gluNewQuadric();
        gluQuadricDrawStyle(sphere, GLU_FILL);
        gluQuadricTexture(sphere, GL_TRUE);
        gluQuadricNormals(sphere, GLU_SMOOTH);
        glColor3f(1.0, 1.0, 1.0);
        
        glPushMatrix();
        glRotatef(-90, 1.0, 0.0, 0.0); 
         gluSphere(sphere, 0.5, 50, 50);
         glPopMatrix();
         
        if(m_godRays)
        {
            glEnable(GL_LIGHTING);
        }
         
         if(m_planet)
         {
             glEnable(GL_LIGHTING);
            m_light->enlighten();
            glBindTexture(GL_TEXTURE_2D, 0);
             glTranslatef(1.5, 0.0, 0.0);
            gluQuadricDrawStyle(sphere, GLU_FILL);
            gluQuadricTexture(sphere, GL_FALSE);
            gluQuadricNormals(sphere, GLU_SMOOTH);
            glColor3f(0.2, 0.2, 0.2);
             gluSphere(sphere, 0.05, 50, 50);
         }
         
         glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-m_width, m_width, -1.0, 1.0, -10.0, 10.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
         
         glPopMatrix();
         m_gRayFrameBuffer->release();
         glBindTexture(GL_TEXTURE_2D, m_gRayFrameBuffer->texture());
         
         glDisable(GL_LIGHTING);
         
         if(m_godRays) m_gRayShader->bind();
         glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_POLYGON);
        glTexCoord2f(0, 0); glVertex3f(-m_width, -1, 0);
        glTexCoord2f(1, 0); glVertex3f(m_width, -1, 0);
        glTexCoord2f(1, 1); glVertex3f(m_width, 1, 0);
        glTexCoord2f(0, 1); glVertex3f(-m_width, 1, 0);
        glEnd();
        if(m_godRays) m_gRayShader->release();
        glEnable(GL_LIGHTING);
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
    float time = m_clock->getElapsedTime().asSeconds();
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
        m_clock->restart();
    }
    else
    {    
        m_mouseRotY += rot_speed;
        m_time += time_speed;
        m_clock->restart();
    }
}


void GLEngine::resize(int width, int height)
{
    m_width = (height>0) ? (GLfloat)width/height : 1;
    
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, m_width, 0.5, 20.0);
    //glOrtho(-m_width, m_width, -1.0, 1.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    m_texFrameBuffer->resize(width, height);
    m_gRayFrameBuffer->resize(width, height);
    m_texWrappingFrameBuffer->resize(width, height);
    m_equiWarpingFrameBuffer->resize(width, height);
}





