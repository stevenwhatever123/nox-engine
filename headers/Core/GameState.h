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

	typedef Map<String, AudioSource> AudioRepo;
	typedef Map<String, MeshScene> MeshSceneRepo;

	struct GameState {
		FullscreenShader *current_post_processor;
		Renderer *renderer;
		Scene *activeScene;
		AudioRepo audioSources;
		MeshSceneRepo meshScenes;
		Array<Scene *> scenes;

		// Active audio in sequencer
		Array<String> selectedAudio;

		Array<FullscreenShader> post_processors;
		Array<Camera*> cameras;

		// u32 texture_used;
		u32 fullscreen_shader_texture_used;
		f32 light[3];
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
