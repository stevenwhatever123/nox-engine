#pragma once

#include <Core/Types.h>

namespace NoxEngine {

	struct IAudioListener {
	public:
		//vec3 vPos;	// Attach ITransform and get position
		vec3 vVel;
		//vec3 vForward;// Attach ITransform and get euler angles rotation
		//vec3 vUp;		// Attach ITransform and get euler angles rotation
	};
}