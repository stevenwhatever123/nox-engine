#include <Camera.h>

using namespace NoxEngine;

Camera::Camera(vec3 stPos) :
	startCamPos(stPos),
	currCamPos(stPos),
	lookingAt(vec3(0.0f, 0.0f, -1.0f)),
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
	// Update camera
	currCamPos = startCamPos + user_shift;
	// Calculate a matrix out of updated camera

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
	lookingAt = glm::normalize(front);

	glm::vec4 temp(1, 2, 3, 4);
	cameraTransf = glm::lookAt(currCamPos, currCamPos + lookingAt, topPointingAt);

	std::cout << Pitch << "  " << Yaw << std::endl;

	// Rotate cam matrix
	
	/*
	mat4 translate_neg = glm::translate(cameraTransf, -currCamPos);
	mat4 translate_pos = glm::translate(cameraTransf, currCamPos);

	mat4 rotation = glm::rotate(mat4(1), glm::radians(user_rotate.x), vec3(1.0f, 0.0f, 0.0f) );
	rotation = rotation * glm::rotate(mat4(1), glm::radians(user_rotate.y), vec3(0.0f, 1.0f, 0.0f) );
	rotation = rotation * glm::rotate(mat4(1), glm::radians(user_rotate.z), vec3(0.0f, 0.0f, 1.0f) );
	
	
	
	// glm::mat4 rotation = glm::rotate( , glm::radians(user_rotate.x), glm::vec3(0.0f, 1.0f, 0.0f));
	std::cout << Pitch << "  " << Yaw  << std::endl;
	cameraTransf = cameraTransf * translate_neg * rotation * translate_pos;
	*/


}


vec3 Camera::getLookingAt() {
	glm::vec4 res = glm::vec4(lookingAt, 1.0f);
	res = res * cameraTransf;
	return glm::normalize(glm::vec3(res.x, res.y, res.z));
}

void Camera::moveHorBy(f32 shift) {
	user_shift.x -= shift;
	// generateCameraParameters();
}

void Camera::moveFwdBy(f32 shift) {
	user_shift.z -= shift;
	// generateCameraParameters();
}

void Camera::moveVerBy(f32 shift) {
	user_shift.y -= shift;
	// generateCameraParameters();
}

void Camera::turnHorBy(f32 shift) {
	user_rotate.x += shift;
	// generateCameraParameters();
}

void Camera::turnVerBy(f32 shift) {
	user_rotate.y += shift;
	// generateCameraParameters();
}





void Camera::turndirUP(f32 shift) {
	Pitch += shift;
	generateCameraParameters();
}

void Camera::turndirDOWN(f32 shift) {
	Pitch -= shift;
	generateCameraParameters();
}

void Camera::turndirLEFT(f32 shift) {
	Yaw -= shift;
	generateCameraParameters();
}

void Camera::turndirRIGHT(f32 shift) {
	Yaw += shift;
	generateCameraParameters();
}
