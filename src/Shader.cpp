#include "Shader.h"
#include "Util.h"
#include <stdlib.h>


Shader::Shader(std::string vert_path, std::string frag_path)
{
    GLuint vertex_handle = compileShader(GL_VERTEX_SHADER, vert_path);
    GLuint frag_handle = compileShader(GL_FRAGMENT_SHADER, frag_path);
    m_program = compileProgram(vertex_handle, frag_handle);
}

void Shader::bind()
{

}

void Shader::release()
{

}

void setUniform1f(char* name, float value)
{
    GLint loc = glGetUniformLocation(m_program, name);
    glUniform1f(loc, value);
}

void setUniform2f(char* name, float v1, float v2)
{
    GLint loc = glGetUniformLocation(m_program, name);
    glUniform2f(loc, v1, v2);
}

void setUniform3f(char* name, float v1, float v2, float v3)
{
    GLint loc = glGetUniformLocation(m_program, name);
    glUniform3f(loc, v1, v2, v3);
}

void setUniform4f(char* name, float v1, float v2, float v3, float v4)
{
    GLint loc = glGetUniformLocation(m_program, name);
    glUniform4f(loc, v1, v2, v3);
}

void setUniform1i(char* name, int value)
{
    GLint loc = glGetUniformLocation(m_program, name);
    glUniform1i(loc, value);
}

void setUniform2i(char* name, int v1, int v2)
{
    GLint loc = glGetUniformLocation(m_program, name);
    glUniform2i(loc, v1, v2);
}

void setUniform3i(char* name, int v1, int v2, int v3)
{
    GLint loc = glGetUniformLocation(m_program, name);
    glUniform3i(loc, v1, v2, v3);
}

void setUniform4i(char* name, int v1, int v2, int v3, int v4)
{
    GLint loc = glGetUniformLocation(m_program, name);
    glUniform4i(loc, v1, v2, v3, v4);
}

//----------------------------------------
//               Protected
//----------------------------------------

GLuint Shader::compileShader(GLEnum shader_type, std::string file_path)
{
    std::string shader_src = readFile(file_path);
    
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, shader_src.c_str());
    glCompileShader(shader);
    printShaderLog(shader, file_path);
    return shader
}

GLuint Shader::compileProgram(GLUint vert_handle, GLUint frag_handle)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vert_handle);
    glAttachShader(program, frag_handle);
    glLinkProgram(program);
    printProgramLog(program);
    return program;
}

void Shader::printShaderLog(GLuint shader, std::string shader_file)
{
    char* text;
    int len;
    
    glGetShaderInfoLog(shader, 4096, len&, text);
    if(len > 0)
    {
        print("Error compiling shader.");
        print(shader_file);
        print(text);
        exit(1);
    }
}

void Shader::printProgramLog(GLuint program)
{
    char* text;
    int len;
    
    glGetProgramInfoLog(program, 4096, len&, text);
    
    if(len > 0)
    {
        print("Error linking program.");
        print(text);
        exit(1);
    }
}













