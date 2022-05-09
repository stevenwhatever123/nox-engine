#pragma once

#include "Renderer.h"
#include "Types.h"
#include "MeshScene.h"
#include "Scene.h"

namespace NoxEngine {

	struct AudioSource {
		String name;
		String file;
		vec3 position;
		f32 sourceVolume;
	};

	typedef std::map<String, AudioSource> AudioRepo;
	typedef std::map<String, MeshScene> MeshSceneRepo;

	struct GameState {
		AudioRepo audioSources;
		Renderer *renderer;
		MeshSceneRepo meshScenes;
		f32 light[3];
		Array<Scene *> scenes;
		Scene *activeScene;
	};



}
