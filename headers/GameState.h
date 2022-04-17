#pragma once

#include <string>
#include <Renderer.h>
#include <Types.h>
#include <Mesh.h>

namespace NoxEngine {

	struct AudioSource {
		std::string name;
		std::string file;
		glm::vec3 position;
		f32 sourceVolume;
	};

	typedef std::map<std::string, AudioSource> AudioRepo;
	typedef std::map<std::string, Mesh> MeshRepo;

	struct GameState {
		AudioRepo audioSources;
		Renderer *renderer;
		MeshRepo meshes;
		f32 light[3];
	};



}
