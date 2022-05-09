#pragma once

#include <Types.h>

namespace NoxEngine {

	struct IAudioSource {
	public:
		//vec3 vPos;	// Attach ITransform and get position
		String filePath;
		float volume;
		bool is3D;
		int channelGroupId;
		Array<int> dspChain;
	};
}