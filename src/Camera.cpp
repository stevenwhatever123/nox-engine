#include <Camera.h>

using namespace NoxEngine;

Camera::Camera(vec3 stPos, vec3 lookAt) :
	startCamPos(stPos),
	currCamPos(stPos),
	lookingAt(lookAt),
	topPointingAt(vec3(0.0f, 1.0f, 0.0f)),
	user_shift(vec3(0.0f)),
	user_rotate(vec2(0.0f))
{
	generateCameraParameters();
}


void Camera::generateCameraParameters() {
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

void Camera::moveHorBy(float shift) {
	user_shift.x -= shift;
	// generateCameraParameters();
}

void Camera::moveFwdBy(float shift) {
	user_shift.z -= shift;
	// generateCameraParameters();
}

void Camera::moveVerBy(float shift) {
	user_shift.y -= shift;
	// generateCameraParameters();
}

void Camera::turnHorBy(float shift) {
	user_rotate.x += shift;
	// generateCameraParameters();
}

void Camera::turnVerBy(float shift) {
	user_rotate.y += shift;
	// generateCameraParameters();
}
