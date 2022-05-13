#pragma once

#include "Renderer.h"
#include "Types.h"
#include "MeshScene.h"
#include "Scene.h"

#include <FullscreenShader.h>

namespace NoxEngine {

	struct AudioSource {
		String name;
		String file;
		vec3 position;
		f32 sourceVolume;
		bool is3d;
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
		u32 win_height;
		u32 win_width;

		// Active audio in sequencer
		Array<String> selectedAudio;

		GLuint texture_used;
		Array<FullscreenShader> post_processors;
		FullscreenShader *current_post_processor;

	};



}
