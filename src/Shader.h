#ifndef SHADER_H
#define SHADER_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <string>

class Shader
{
    public:
        Shader(std::string vert_path, std::string frag_path);
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
        GLuint compileShader(GLEnum shader_type, std::string file_path);
        GLuint compileProgram(GLuint vert_handle, GLuint frag_handle);
        void printShaderLog(GLuint shader_handle, std::string shader_path);
        void printProgramLog(GLuint program_handle);
        
    private:
        GLuint m_program;
};

#endif
