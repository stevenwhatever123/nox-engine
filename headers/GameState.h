#pragma once

#include <string>
#include <Renderer.h>
#include <Types.h>

namespace NoxEngine {

	struct AudioSource {
		std::string name;
		std::string file;
		glm::vec3 position;
		f32 sourceVolume;
	};

	typedef std::map<std::string, AudioSource> AudioRepo;

	struct GameState {
		AudioRepo audioSources;
		Renderer *renderer;
		f32 light[3];
	};
}
