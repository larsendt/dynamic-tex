#include "GLEngine.h"
#include "Util.h"

GLEngine::GLEngine(int argc, char** argv)
{
	print("Initializing GLEngine");
	initGL(argc, argv);
	resize(800, 600);
}

void GLEngine::initGL(int argc, char** argv)
{
	m_window = new sf::Window(sf::VideoMode(800, 600, 32), "GLEngine");
	m_clock = new sf::Clock();
	glClearColor(0.0, 0.0, 0.0, 1.0);
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
			else if(Event.Type == sf::Event::KeyPressed)
			{
				if(Event.Key.Code == sf::Key::Escape)
				{
					m_window->Close();
					return 0;
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
	
	
	glFlush();
}


void GLEngine::update()
{
	float time = m_clock->GetElapsedTime();
    m_clock->Reset();
    
    if(time < m_updateRate)
    	return;
}


void GLEngine::resize(int width, int height)
{
	m_width = (height>0) ? (GLfloat)width/height : 1;
	
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-m_width, m_width, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}




