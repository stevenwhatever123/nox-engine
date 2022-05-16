#pragma once

#include "Renderer.h"
#include "Types.h"
#include "MeshScene.h"
#include "Scene.h"

#include <FullscreenShader.h>

namespace NoxEngine {

	// forward declares
	class Entity;

	struct AudioSource {
		String name;
		String file;
		vec3 position;
		f32 sourceVolume;
		bool is3d;
	};

	typedef std::map<String, MeshScene> MeshSceneRepo;

	struct GameState {
		FullscreenShader *current_post_processor;
		Renderer *renderer;
		Scene *activeScene;
		Entity* activeAudioListener;	// Entity is needed because IAudioListener and ITransform are both needed
		Array<Scene *> scenes;
		MeshSceneRepo meshScenes;

		// Active audio in sequencer
		Array<String> selectedAudio;

		Array<FullscreenShader> post_processors;
		Array<Camera*> cameras;

		// u32 texture_used;
		u32 fullscreen_shader_texture_used;
		Array<vec3> lightSources;
		u32 prev_win_height;
		u32 prev_win_width;
		u32 win_height;
		u32 win_width;

		i8 mouse_left;
		i8 mouse_right;

		f64 mouse_x_delta;
		f64 mouse_y_delta;
		f64 mouse_x;
		f64 mouse_y;


	};



}
