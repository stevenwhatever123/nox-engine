/*
 * A camera class
*/

#pragma once

#include <cstdio>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class Camera
{
private:

    glm::vec3 lookingAt;
    glm::vec3 topPointingAt;

    glm::mat4 cameraTransf;

    glm::vec3 user_shift = glm::vec3(0.0f);
    glm::vec2 user_rotate = glm::vec2(0.0f);


    void generateCameraParameters()
    {
        // Update camera
        currCamPos = startCamPos + user_shift;

        // Calculate a matrix out of updated camera
        cameraTransf = glm::lookAt(currCamPos, currCamPos + lookingAt, topPointingAt);

        // Rotate cam matrix
        glm::mat4 rotation = glm::rotate(glm::rotate(glm::mat4(1.0f), glm::radians(user_rotate.y), glm::vec3(1.0f, 0.0f, 0.0f)),
                                        glm::radians(user_rotate.x), glm::vec3(0.0f, 1.0f, 0.0f));
        cameraTransf = rotation * cameraTransf;

    }


public:

    glm::vec3 startCamPos;
    glm::vec3 currCamPos;

    Camera();

    Camera(glm::vec3 stPos) :
        startCamPos(stPos),
        currCamPos(stPos),
        lookingAt(glm::vec3(0.0f, 0.0f, -1.0f)),
        topPointingAt(glm::vec3(0.0f, 1.0f, 0.0f))
    {
        generateCameraParameters();
    }


    inline glm::mat4 getCameraTransf() const { return cameraTransf; }

    glm::vec3 getLookingAt()
    {
        glm::vec4 res = glm::vec4(lookingAt, 1.0f);
        res = res * cameraTransf;
        return glm::normalize(glm::vec3(res.x, res.y, res.z));
    }


    void moveHorBy(float shift)
    {
        user_shift.x -= shift;
        generateCameraParameters();
    }

    void moveFwdBy(float shift)
    {
        user_shift.z -= shift;
        generateCameraParameters();
    }

    void moveVerBy(float shift)
    {
        user_shift.y -= shift;
        generateCameraParameters();
    }



    void turnHorBy(float shift)
    {
        user_rotate.x += shift;
        generateCameraParameters();
    }

    void turnVerBy(float shift)
    {
        user_rotate.y += shift;
        generateCameraParameters();
    }



};
