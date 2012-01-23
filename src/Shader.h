#ifndef SHADER_H
#define SHADER_H

#define USE_GLEW
#ifdef USE_GLEW
#include <GL/glew.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <string>

class Shader
{
    public:
        Shader(char* vert_path, char* frag_path);
        void bind();
        void release();
        
        void setUniform1f(char* name, float value);
        void setUniform2f(char* name, float v1, float v2);
        void setUniform3f(char* name, float v1, float v2, float v3);
        void setUniform4f(char* name, float v1, float v2, float v3, float v4);
        
        void setUniform1i(char* name, int value);
        void setUniform2i(char* name, int v1, int v2);
        void setUniform3i(char* name, int v1, int v2, int v3);
        void setUniform4i(char* name, int v1, int v2, int v3, int v4);

    protected:
        GLuint compileShader(GLenum shader_type, char* file_path);
        GLuint linkProgram(GLuint vert_handle, GLuint frag_handle);
        void printShaderLog(GLuint shader_handle, char* shader_path);
        void printProgramLog(GLuint program_handle);
        
    private:
        GLuint m_program;
};

#endif
