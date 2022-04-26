#ifndef  SKYBOX_H
#define SKYBOX_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>

#endif
//#include "camera.h"

int WIDTH = 800;
int HEIGHT = 600;

float DeltaTime = 0.0;
float LastFrame = 0.0;

//Camera camera;
float LastXPosition, LastYPosition;
bool flag = true;

//to control cusor's position
//void CursorPosCallback(GLFWwindow* window, double xposition, double yposition)
//{
//    float CurrentXPosition = static_cast<float>(xposition);
//    float CurrentYPosition = static_cast<float>(yposition);
//    if (flag == true)
//    {
//        LastXPosition = CurrentXPosition;
//        LastYPosition = CurrentYPosition;
//        flag = false;
//    }
//
//    float xoffset = CurrentXPosition - LastXPosition;
//    float yoffset = LastYPosition - CurrentYPosition;
//
//    camera.MouseMovement(xoffset, yoffset);
//
//    LastXPosition = CurrentXPosition;
//    LastYPosition = CurrentYPosition;
//
//}
//
////to control mouse wheel
//void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
//{
//    float Yoffset = static_cast<float>(yoffset);
//    camera.MouseScroll(Yoffset);
//
//}

//load textures
unsigned int LoadTextures(std::vector<std::string> images)
{
    unsigned int texture;
    glGenTextures(1, &texture);

    int width, height, channels;
    unsigned char* TextureData = stbi_load(images[0].c_str(), &width, &height, &channels, 0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureData);
    TextureData = stbi_load(images[1].c_str(), &width, &height, &channels, 0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureData);
    TextureData = stbi_load(images[2].c_str(), &width, &height, &channels, 0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureData);
    TextureData = stbi_load(images[3].c_str(), &width, &height, &channels, 0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureData);
    TextureData = stbi_load(images[4].c_str(), &width, &height, &channels, 0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureData);
    TextureData = stbi_load(images[5].c_str(), &width, &height, &channels, 0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureData);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}