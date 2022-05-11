#pragma once

#include <vector>

#include "Renderer.h"
#include "Types.h"
#include "MeshScene.h"
#include "Scene.h"

namespace NoxEngine {

	// forward declares
	class IAudioListener;

	struct AudioSource {
		String name;
		String file;
		vec3 position;
		f32 sourceVolume;
		bool is3d;
	};

	typedef std::map<String, MeshScene> MeshSceneRepo;

	struct GameState {
		Renderer *renderer;
		MeshSceneRepo meshScenes;
		f32 light[3];
		IAudioListener* activeAudioListener;
		Array<Scene *> scenes;
		Scene *activeScene;

		// Active audio in sequencer
		std::vector<std::string> selectedAudio;
	};



}
