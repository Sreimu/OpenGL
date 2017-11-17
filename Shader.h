//
// Created by leisirui on 2017/10/31.
//

#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>

#include <GL/glew.h>

class Shader{
public :
    GLuint Program;
    Shader(const GLchar* vertexShaderPath, const GLchar* fragmentShaderPath);
    void Use();

    GLint GetUniformLocation(const GLchar* name);
};

#endif //OPENGL_SHADER_H
