#pragma once

#include <string>
#include <vector>
#include <Core/Renderer.h>
#include <Core/Types.h>
#include <Core/MeshScene.h>
#include <Core/Scene.h>

namespace NoxEngine {

	struct AudioSource {
		std::string name;
		std::string file;
		glm::vec3 position;
		f32 sourceVolume;
	};

	typedef std::map<std::string, AudioSource> AudioRepo;
	typedef std::map<std::string, MeshScene> MeshSceneRepo;

	struct GameState {
		AudioRepo audioSources;
		Renderer *renderer;
		MeshSceneRepo meshScenes;
		f32 light[3];
		Array<Scene *> scenes;
		Scene *activeScene;

		// Active audio in sequencer
		std::vector<std::string> selectedAudio;
	};



}
