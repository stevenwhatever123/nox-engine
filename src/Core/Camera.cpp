#include <Core/Camera.h>

using namespace NoxEngine;

Camera::Camera(vec3 stPos, vec3 lookAt) :
	startCamPos(stPos),
	currCamPos(stPos),
	lookingAt(lookAt),
	topPointingAt(vec3(0.0f, 1.0f, 0.0f)),
	user_shift(vec3(0.0f)),
	user_rotate(vec3(0.0f)),
	fov(50)
{	

	Yaw = 0.0;
	Pitch = -30.0;

	user_rotate.y = 0;
	generateCameraParameters();
}


void Camera::generateCameraParameters() {

	// // Update camera
	// currCamPos = startCamPos + user_shift;
	// // Calculate a matrix out of updated camera

	// if (Pitch > 89.0) {
	// 	Pitch = 89.0;
	// }

	// if (Pitch < -89.0) {
	// 	Pitch = -89.0;
	// }
	
	// glm::vec3 front;
	// float RadYaw = glm::radians(Yaw);
	// float RadPitch = glm::radians(Pitch);
	// front.x = cos(RadYaw) * cos(RadPitch);
	// front.y = sin(RadPitch);
	// front.z = sin(RadYaw) * cos(RadPitch);
	// lookingAt = glm::normalize(front);

	// cameraTransf = glm::lookAt(currCamPos, currCamPos + lookingAt, topPointingAt);
	// // cameraTransf[3][0] = 0;
	// // cameraTransf[3][1] = 0;
	// // cameraTransf[3][2] = 0;

	// Update camera
	currCamPos = startCamPos + user_shift;

	// Calculate a matrix out of updated camera
	cameraTransf = glm::lookAt(currCamPos, lookingAt, topPointingAt);

	// Rotate cam matrix
	glm::mat4 rotation = glm::rotate(glm::rotate( glm::mat4(1.0f), glm::radians(user_rotate.y), glm::vec3(1.0f, 0.0f, 0.0f) ), glm::radians(user_rotate.x), glm::vec3(0.0f, 1.0f, 0.0f));
	cameraTransf = rotation * cameraTransf;

}


vec3 Camera::getLookingAt() {
	glm::vec4 res = glm::vec4(lookingAt, 1.0f);
	res = res * cameraTransf;
	return glm::normalize(glm::vec3(res.x, res.y, res.z));
}

void Camera::moveHorBy(f32 shift) {
	user_shift.x -= shift;
}

void Camera::moveFwdBy(f32 shift) {
	user_shift.z -= shift;
}

void Camera::moveVerBy(f32 shift) {
	user_shift.y -= shift;
}

void Camera::turnHorBy(f32 shift) {
	user_rotate.x += shift;
}

void Camera::turnVerBy(f32 shift) {
	user_rotate.y += shift;
}

void Camera::turndirUP(f32 shift) {
	Pitch += shift;
}

void Camera::turndirDOWN(f32 shift) {
	Pitch -= shift;
}

void Camera::turndirLEFT(f32 shift) {
	Yaw -= shift;
}

void Camera::turndirRIGHT(f32 shift) {
	Yaw += shift;
}
