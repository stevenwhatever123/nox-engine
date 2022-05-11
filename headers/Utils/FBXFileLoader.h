#pragma once

#include <assimp/Importer.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <Core/MeshScene.h>

namespace NoxEngine {
	const aiScene* readFBX(const char* filename);

	aiScene* generateAiScene(const MeshScene& meshScene);
	void exportFBX(aiScene *scene);
};

