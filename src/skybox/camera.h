#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Camera
{
public:

    glm::vec3 Position;
    glm::vec3 Up;
    glm::vec3 Front;
    float Yaw, Pitch, Sensitivity, Zoom;

    //constructor
    Camera()
    {
        Position = glm::vec3(0.0, 0.0, 0.0);
        Up = glm::vec3(0.0, 1.0, 0.0);
        Front = glm::vec3(1.0, 0.0, 0.0);

        Yaw = 0.0;
        Pitch = 0.0;
        Sensitivity = 0.1;
        Zoom = 50.0;
    }

    //processes input from mouse
    void MouseMovement(float Xoffset, float Yoffset)
    {

        Yaw = Yaw + Xoffset * Sensitivity;
        Pitch = Pitch + Yoffset * Sensitivity;

        if (Pitch > 89.0) {
            Pitch = 89.0;
        }

        if (Pitch < -89.0) {
            Pitch = -89.0;
        }

        glm::vec3 front;
        float RadYaw = glm::radians(Yaw);
        float RadPitch = glm::radians(Pitch);
        front.x = cos(RadYaw) * cos(RadPitch);
        front.y = sin(RadPitch);
        front.z = sin(RadYaw) * cos(RadPitch);
        Front = glm::normalize(front);
    }

    //processes input form mouse scroll-wheel.
    void MouseScroll(float Yoffset)
    {
        Zoom = Zoom - Yoffset;

        if (Zoom < 0.0) {
            Zoom = 0.0;
        }

        if (Zoom > 50.0) {
            Zoom = 50.0;
        }
    }
};
#endif