#pragma once

#include <assimp/Importer.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include "Mesh.h"
#include <MeshScene.h>

namespace NoxEngine {
	const aiScene* readFBX(const char* filename);
	Mesh * getMesh(const aiScene* pScene);

	aiScene* generateAiScene(const MeshScene& meshScene);
	void exportFBX(aiScene *scene);
};

