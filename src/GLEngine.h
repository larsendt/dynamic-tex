#ifndef GLENGINE_H
#define GLENGINE_H

#define USE_GLEW
#ifdef USE_GLEW
#include <GL/glew.h>
#endif

#include <SFML/Window.hpp>
#include "Shader.h"
#include "FrameBuffer.h"

class GLEngine
{
	public:
		GLEngine(int argc, char** argv);
		void initGL(int argc, char** argv);
		void drawScene();
		void update();
		void resize(int width, int height);
		int begin();
		
	private:
		sf::Window* m_window;
		sf::Clock* m_clock;
		Shader* m_texShader;
		Shader* m_gRayShader;
		FrameBuffer* m_texFrameBuffer;
		FrameBuffer* m_gRayFrameBuffer;
		float m_updateRate;
		float m_width;
		bool m_fullScreen;
		bool m_sphere;
		bool m_godRays;
		GLuint m_texture;
		int m_mouseLastX;
		int m_mouseLastY;
        float m_mouseRotX;
        float m_mouseRotY;
        float m_time;
        float m_scale;
        int m_screenWidth;
        int m_screenHeight;
};

#endif
