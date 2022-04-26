#include <Camera.h>

using namespace NoxEngine;

Camera::Camera(vec3 stPos) :
	startCamPos(stPos),
	currCamPos(stPos),
	lookingAt(vec3(0.0f, 0.0f, -1.0f)),
	topPointingAt(vec3(0.0f, 1.0f, 0.0f)),
	user_shift(vec3(0.0f)),
	user_rotate(vec3(0.0f))
{
	user_rotate.y = 0;
	generateCameraParameters();
}


void Camera::generateCameraParameters() {
	// Update camera
	currCamPos = startCamPos + user_shift;
	// Calculate a matrix out of updated camera
	cameraTransf = glm::lookAt(currCamPos, currCamPos + lookingAt, topPointingAt);
	// Rotate cam matrix
	
	mat4 translate_neg = glm::translate(cameraTransf, -currCamPos);

	mat4 rotation = glm::rotate(mat4(1), glm::radians(user_rotate.x), vec3(1.0f, 0.0f, 0.0f) );
	rotation = rotation*glm::rotate(mat4(1), glm::radians(user_rotate.y), vec3(0.0f, 1.0f, 0.0f) );
	rotation = rotation*glm::rotate(mat4(1), glm::radians(user_rotate.z), vec3(0.0f, 0.0f, 1.0f) );

	mat4 translate_pos = glm::translate(cameraTransf, currCamPos);
	// glm::mat4 rotation = glm::rotate( , glm::radians(user_rotate.x), glm::vec3(0.0f, 1.0f, 0.0f));

	cameraTransf = cameraTransf*translate_neg*rotation*translate_pos;

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
