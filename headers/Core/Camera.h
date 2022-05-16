#pragma once

#include <Core/Types.h>

namespace NoxEngine {

	class Camera {
		public:
			Camera(vec3 stPos, vec3 lookAt = vec3(0.0f, 0.0f, 0.0f));

			inline mat4 getCameraTransf() {

				generateCameraParameters();

				return cameraTransf;
			}

			inline vec3& GetCameraYawPitchRoll() { return yaw_pitch_roll; }
			inline vec3& GetCameraPosition() { return pos; }
			inline vec3& GetCameraUp()      { return forward; }
			inline vec3& GetCameraForward() { return up; }

			void moveHorBy(f32 shift);
			void moveFwdBy(f32 shift);
			void moveVerBy(f32 shift);
			void turnHorBy(f32 shift);
			void turnVerBy(f32 shift);

			void turndirUP(f32 shift);
			void turndirDOWN(f32 shift);
			void turndirLEFT(f32 shift);
			void turndirRIGHT(f32 shift);

			void moveToMousePos(f32 x, f32 y);

			void changePitch(f32 amount);
			void setPitch(f32 amount);
			f32 getPitch() const { return yaw_pitch_roll.y; };

			void changeYaw(f32 amount);
			void setYaw(f32 amount);
			f32 getYaw() const { return yaw_pitch_roll.x; };

			void changeRoll(f32 amount);

			vec3 getLookingAt();

			vec3 pos;
			vec3 forward;
			vec3 up;
			vec3 user_shift;
			vec3 yaw_pitch_roll;
			vec3 yaw_pitch_roll_shift;

			f32 fov;

		private:
			mat4 cameraTransf;
			vec3 user_rotate;
			vec3 startCamPos;

			void generateCameraParameters();

		
	};
}
