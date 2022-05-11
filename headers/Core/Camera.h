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

			inline vec3& GetCameraPosition() { return currCamPos; }

			void moveHorBy(f32 shift);
			void moveFwdBy(f32 shift);
			void moveVerBy(f32 shift);
			void turnHorBy(f32 shift);
			void turnVerBy(f32 shift);
			vec3 getLookingAt();

			vec3 currCamPos;
			vec3 user_shift;

		private:
			vec3 lookingAt;
			vec3 topPointingAt;
			mat4 cameraTransf;
			vec2 user_rotate;
			vec3 startCamPos;

			void generateCameraParameters();

		
	};
}
