#pragma once

#include <Types.h>

namespace NoxEngine {

	struct IAudioListener {
	public:
		//vec3 vPos;	// Attach ITransform and get position
		vec3 vVel;
		vec3 vForward;
		vec3 vUp;
	};
}