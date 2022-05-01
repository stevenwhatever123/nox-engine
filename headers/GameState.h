#pragma once

#include <Renderer.h>
#include <Types.h>
#include <Mesh.h>
#include <MeshScene.h>
#include <Scene.h>
#include <FullscreenShader.h>

namespace NoxEngine {

	struct AudioSource {
		String name;
		String file;
		glm::vec3 position;
		f32 sourceVolume;
	};

	typedef Map<String, AudioSource> AudioRepo;
	typedef Map<String, Mesh> MeshRepo;
	typedef Map<String, MeshScene> MeshSceneRepo;

	struct GameState {
		AudioRepo audioSources;
		Renderer *renderer;
		MeshRepo meshes;
		MeshSceneRepo meshScenes;
		f32 light[3];
		Array<Scene *> scenes;
		Scene *activeScene;
		GLuint texture_used;
		Array<FullscreenShader> post_processors;
		FullscreenShader& current_post_processor;
	};

}
