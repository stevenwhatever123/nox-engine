#pragma once

#include <string>
#include <Renderer.h>
#include <Types.h>
#include <Mesh.h>
#include <MeshScene.h>
#include <Scene.h>

namespace NoxEngine {

	struct AudioSource {
		std::string name;
		std::string file;
		glm::vec3 position;
		f32 sourceVolume;
	};

	typedef std::map<std::string, AudioSource> AudioRepo;
	typedef std::map<std::string, Mesh> MeshRepo;
	typedef std::map<std::string, MeshScene> MeshSceneRepo;

	struct GameState {
		AudioRepo audioSources;
		Renderer *renderer;
		MeshRepo meshes;
		MeshSceneRepo meshScenes;
		f32 light[3];
		Array<Scene *> scenes;
		Scene *activeScene;
	};



}
