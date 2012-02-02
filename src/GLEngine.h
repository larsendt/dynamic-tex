#ifndef GLENGINE_H
#define GLENGINE_H

#include <SFML/Window.hpp>
#include "Shader.h"
#include "FrameBuffer.h"
#include "Light.h"

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
		Light* m_light;
		Shader* m_gRayShader;
		Shader* m_texWrappingShader;
		Shader* m_equiWarpingShader;
		Shader* m_currentColorShader;
        std::vector<Shader*> m_colorShaders;
		FrameBuffer* m_texFrameBuffer;
		FrameBuffer* m_gRayFrameBuffer;
		FrameBuffer* m_texWrappingFrameBuffer;
		FrameBuffer* m_equiWarpingFrameBuffer;
		float m_updateRate;
		float m_width;
		bool m_fullScreen;
		bool m_sphere;
		bool m_godRays;
		int m_shaderIndex;
		bool m_wrapTex;
		bool m_planet;
		bool m_equiWarp;
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
