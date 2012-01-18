#ifndef GLENGINE_H
#define GLENGINE_H

#include <SFML/Window.hpp>

class GLEngine
{
	public:
		GLEngine(int argc, char** argv);
		void initGL(int argc, char** argv);
		void drawScene();
		void update();
		void resize(int width, int height);
		void pan(bool x, bool y);
		int begin();
		
	private:
		sf::Window* m_window;
		sf::Clock* m_clock;
		float m_updateRate;
		float m_width;
};

#endif
