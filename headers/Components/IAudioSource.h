#pragma once

#include <Core/Types.h>

namespace NoxEngine {

	struct IAudioSource {
	public:
		//vec3 vPos;	// Attach ITransform and get position
		String filePath;
		float volume;
		bool is3D;
		bool isLooping;
		bool isStream;
		int channelGroupId;
		Array<int> dspChain;

		bool loaded;
	};
}