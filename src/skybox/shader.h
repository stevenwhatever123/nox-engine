#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int Temp;

    //constructor
    Shader(const char* vertexShaderPath, const char* fragmentShaderPath)// constructor
    {
        //vertex shader
        std::ifstream VertexShaderFile;
        std::stringstream VertexShaderStream;
        std::string VertexShaderCode;
        const char* VertexShader;
        unsigned int vertex;

        VertexShaderFile.open(vertexShaderPath);
        VertexShaderStream << VertexShaderFile.rdbuf();
        VertexShaderFile.close();
        VertexShaderCode = VertexShaderStream.str();
        VertexShader = VertexShaderCode.c_str();
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &VertexShader, NULL);
        glCompileShader(vertex);


        //fragment shader
        std::ifstream FragmentShaderFile;
        std::stringstream FragmentShaderStream;
        std::string FragmentShaderCode;
        const char* FragmentShader;
        unsigned int fragment;

        FragmentShaderFile.open(fragmentShaderPath);
        FragmentShaderStream << FragmentShaderFile.rdbuf();
        FragmentShaderFile.close();
        FragmentShaderCode = FragmentShaderStream.str();
        FragmentShader = FragmentShaderCode.c_str();
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &FragmentShader, NULL);
        glCompileShader(fragment);

        Temp = glCreateProgram();
        glAttachShader(Temp, vertex);
        glAttachShader(Temp, fragment);
        glLinkProgram(Temp);

    }

    //Specifies a program object handle with an executable object
    void use()
    {
        glUseProgram(Temp);
    }

    //Pass variable values into the render pipeline
    void setMat4(const std::string& name, const glm::mat4& matrix)
    {
        glUniformMatrix4fv(glGetUniformLocation(Temp, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
    }


};
#endif
