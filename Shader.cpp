//
// Created by leisirui on 2017/10/31.
//

#include "Shader.h"
using namespace std;

Shader::Shader(const GLchar *vertexShaderPath, const GLchar *fragmentShaderPath) {
    ifstream vShaderFile;
    ifstream fShaderFile;
    string vShader;
    string fShader;

    vShaderFile.open(vertexShaderPath);
    fShaderFile.open(fragmentShaderPath);

    vShaderFile.exceptions(ifstream::badbit);
    fShaderFile.exceptions(ifstream::badbit);

    stringstream vShaderStream, fShaderStream;
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vShaderFile.close();
    fShaderFile.close();

    vShader = vShaderStream.str();
    fShader = fShaderStream.str();

    const GLchar* vShaderCode = vShader.c_str();
    const GLchar* fShaderCode = fShader.c_str();

    GLuint vertexShader;
    GLuint fragmentShader;

    GLint success;
    GLchar infoLog[512];

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    this->Program = glCreateProgram();
    glAttachShader(this->Program, vertexShader);
    glAttachShader(this->Program, fragmentShader);
    glLinkProgram(this->Program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Use() {

    glUseProgram(this->Program);

}

GLint Shader::GetUniformLocation(const GLchar* name)
{
    return glGetUniformLocation(this->Program, name);
}