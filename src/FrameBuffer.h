#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#define USE_GLEW
#ifdef USE_GLEW
#include <GL/glew.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

class FrameBuffer
{
    public:
        FrameBuffer(int width, int height);
        void bind();
        void release();
        void resize(int width, int height);
        GLuint texture();
        
    protected:
        void deleteBuffers();
        void createBuffers();
    
    private:
        GLuint m_texture;
        GLuint m_depthBuffer;
        GLuint m_frameBuffer;
        int m_width;
        int m_height;
};

#endif
