#include <Core/Camera.h>
#include <glm/gtx/euler_angles.hpp>
#include <Utils/Utils.h>

using namespace NoxEngine;

Camera::Camera(vec3 stPos, vec3 lookAt) :
	startCamPos(stPos),
	pos(stPos),
	forward(glm::normalize(lookAt - stPos)),
	up(vec3(0.0f, 1.0f, 0.0f)),
	user_rotate(vec3(0.0f)),
	yaw_pitch_roll(0),
	fov(50),
	user_shift(0),
	sensetivity(10)
{	
	cameraTransf = glm::lookAt(pos, lookAt, up);
	generateCameraParameters();
}

void Camera::generateCameraParameters() {
	// Calculate a matrix out of updated camera

	// mat4 rotation = glm::eulerAngleXYZ(yaw_pitch_roll.x, yaw_pitch_roll.y, yaw_pitch_roll.z);
	// cameraTransf = glm::translate(cameraTransf, -startCamPos);
	// cameraTransf = cameraTransf*rotation;
	// cameraTransf = glm::translate(cameraTransf, startCamPos);
	// cameraTransf = glm::translate(cameraTransf, user_shift);


	cameraTransf = glm::lookAt(pos, pos + forward, up);

	pos += user_shift;

	user_shift.x = 0;
	user_shift.z = 0;
	user_shift.y = 0;

	yaw_pitch_roll_shift.x = 0;
	yaw_pitch_roll_shift.z = 0;
	yaw_pitch_roll_shift.y = 0;
}


vec3 Camera::getLookingAt() {
	return glm::normalize(forward);
}

void Camera::moveHorBy(f32 shift) {
	vec3 v = glm::normalize(
		glm::cross(
			glm::normalize(forward),
			glm::normalize(up)
		)
	)*shift;
	user_shift += v;
}

void Camera::moveFwdBy(f32 shift) {
	user_shift += forward*shift;
}

void Camera::moveVerBy(f32 shift) {
	user_shift += up*shift;
}

void Camera::moveToMousePos(f64 pitch, f64 yaw) {
	changePitch((f32)pitch);
	changeYaw((f32)yaw);
}

void Camera::changeYaw(f32 amount) {
	yaw_pitch_roll.x += amount;
	yaw_pitch_roll_shift.x = amount;
	mat3 rotation = glm::rotate(
		mat4(1.0f),
		10*glm::radians( yaw_pitch_roll_shift.x ),
		glm::normalize(glm::cross(glm::normalize(forward), glm::normalize(up))));
	forward = rotation*forward;
}

void Camera::changePitch(f32 amount) {
	yaw_pitch_roll.y += amount;
	yaw_pitch_roll_shift.y = amount;
	mat3 rotation = glm::eulerAngleY(10*glm::radians( yaw_pitch_roll_shift.y ));
	forward = rotation*forward;
}

void Camera::changeRoll(f32 amount) {
	yaw_pitch_roll.z += amount;
	yaw_pitch_roll_shift.z = amount;
	mat3 rotation = glm::eulerAngleZ(yaw_pitch_roll_shift.z);
	up = rotation*up;
}

void Camera::setYaw(f32 amount) {
	f32 change_by = glm::abs(yaw_pitch_roll.x - amount);
	if(amount < yaw_pitch_roll.x) change_by *= -1;
	changeYaw(change_by);
}

void Camera::setPitch(f32 amount) {
	f32 change_by = glm::abs(yaw_pitch_roll.y - amount);
	if(amount < yaw_pitch_roll.y) change_by *= -1;
	changePitch(change_by);
}
