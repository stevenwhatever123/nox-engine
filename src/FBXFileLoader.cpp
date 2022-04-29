#include "FBXFileLoader.h"
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/gtx/string_cast.hpp>

#include "MeshNode.h"
#include "Mesh.h"
#include <Utils.h>

using NoxEngineUtils::Logger;

const aiScene* NoxEngine::readFBX(const char* filename)
{
	Assimp::Importer importer;

	importer.ReadFile(filename,
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs |
		aiProcess_JoinIdenticalVertices);

	// The code above only returns a pointer to the imported data
	// It will be destroyed after exiting the block
	// Therefore, we use GetOrphanedScene to get ownership
	aiScene* pScene = importer.GetOrphanedScene();

	// Check if the file contains any animation
	// Otherwise we read again and apply aiProcess_PreTransformVertices
	if (pScene)
	{
		if (pScene->mNumAnimations < 1)
		{
			std::cout << "HIU: " << pScene->mNumAnimations << "\n";
			//delete pScene;
			importer.ReadFile(filename,
				aiProcess_Triangulate |
				aiProcess_GenSmoothNormals |
				aiProcess_FlipUVs |
				aiProcess_JoinIdenticalVertices | 
				aiProcess_PreTransformVertices);

			pScene = importer.GetOrphanedScene();
		}

		NoxEngineUtils::Logger::debug("Success reading '%s'", filename);

		return pScene;
	}
	else
	{
		NoxEngineUtils::Logger::debug("Error parsing '%s': '%s'", filename,
			importer.GetErrorString());

		return nullptr;
	}
}

NoxEngine::Mesh* NoxEngine::getMesh(const aiScene* pScene) {
	return nullptr;
}

