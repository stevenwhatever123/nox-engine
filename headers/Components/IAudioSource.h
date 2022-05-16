#pragma once

#include <Core/Types.h>

namespace NoxEngine {

	class IAudioSource {
	public:
		//vec3 vPos;	// Attach ITransform and get position
		String filePath;
		float volume;
		bool is3D;
		bool isLooping;
		bool isStream;
		int channelId;
		Array<int> dspChain;

		int selectedDspFilter;

		bool loaded;
		bool paused;
		bool stopped;
	};
}
