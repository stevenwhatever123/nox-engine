#include "Utils/FBXFileLoader.h"
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/gtx/string_cast.hpp>
#include <Utils/Utils.h>

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

aiScene* NoxEngine::generateAiScene(const MeshScene& meshScene)
{
	aiScene *scene = new aiScene();

	scene->mRootNode = new aiNode();
	scene->mRootNode->mName = meshScene.nodeHierarchy.name;

	// Material data
	scene->mMaterials = new aiMaterial * [1];
	scene->mMaterials[0] = nullptr;
	scene->mNumMaterials = 1;
	scene->mMaterials[0] = new aiMaterial();

	// Mesh data
	i32 numberOfMeshes = meshScene.meshes.size();
	scene->mMeshes = new aiMesh * [numberOfMeshes];

	for(i32 i = 0; i < numberOfMeshes; i++)
		scene->mMeshes[i] = nullptr;

	scene->mNumMeshes = numberOfMeshes;

	for (i32 i = 0; i < numberOfMeshes; i++)
	{
		scene->mMeshes[i] = new aiMesh();
		scene->mMeshes[0]->mMaterialIndex = 0;
	}

	scene->mRootNode->mMeshes = new u32[numberOfMeshes];
	for (i32 i = 0; i < numberOfMeshes; i++)
		scene->mRootNode->mMeshes[i] = 0;
	scene->mRootNode->mNumMeshes = numberOfMeshes;

	scene->mMetaData = new aiMetadata();
	
	for (i32 i = 0; i < numberOfMeshes; i++)
	{
		auto pMesh = scene->mMeshes[i];
		pMesh->mPrimitiveTypes = aiPrimitiveType_TRIANGLE;

		const auto& vertices = meshScene.meshes[i]->vertices;
		const auto& texCoords = meshScene.meshes[i]->texCoords;

		// Vertices data
		pMesh->mVertices = new aiVector3D[vertices.size()];
		pMesh->mNumVertices = vertices.size();

		// Texture data
		pMesh->mTextureCoords[0] = new aiVector3D[vertices.size()];
		pMesh->mNumUVComponents[0] = vertices.size();

		// Copy vertex data
		for (i32 j = 0; j < vertices.size(); j++)
		{
			pMesh->mVertices[j] = aiVector3D(vertices[j].x, vertices[j].y, vertices[j].z);
			pMesh->mTextureCoords[0][j] = aiVector3D(texCoords[j].x, texCoords[j].y, 0);
		}

		const auto& meshFace = meshScene.meshes[i]->faces;

		// Copy face data
		pMesh->mFaces = new aiFace[meshFace.size()];
		pMesh->mNumFaces = meshFace.size();

		for (i32 j = 0; j < meshFace.size(); j++)
		{
			aiFace& face = pMesh->mFaces[j];

			face.mIndices = new u32[3];
			face.mNumIndices = 3;

			face.mIndices[0] = meshFace[j].x;
			face.mIndices[1] = meshFace[j].y;
			face.mIndices[2] = meshFace[j].z;
		}
	}

	// The above part is still working

	// Node hierarchy

	//Animation data?
	//i32 numOfAnimation = meshScene.getNumOfAnimations();
	//scene->mAnimations = new aiAnimation * [numOfAnimation];
	//scene->mNumAnimations = numOfAnimation;
	//
	//i32 numOfNodes = meshScene.allNodes.size();
	//
	//for (i32 i = 0; i < numOfAnimation; i++)
	//{
	//	auto pAnimation = scene->mAnimations[i];
	//	pAnimation->mChannels = new aiNodeAnim * [numOfNodes];
	//	pAnimation->mNumChannels = numOfNodes;
	//	pAnimation->mDuration = meshScene.animationDuration[i];
	//	pAnimation->mName = meshScene.animations[i]->mName;
	//	pAnimation->mMeshChannels = meshScene.animations[i]->mMeshChannels;
	//	pAnimation->mNumMeshChannels = meshScene.animations[i]->mNumMeshChannels;
	//	pAnimation->mTicksPerSecond = meshScene.animations[i]->mTicksPerSecond;

	//	for (i32 j = 0; j < numOfNodes; j++)
	//	{
	//		auto pNodeAnim = pAnimation[i].mChannels[j];

	//		pNodeAnim->mNodeName = meshScene.allNodes[i]->name;
	//		
	//		i32 numOfKeys = meshScene.allNodes[i]->nodeAnimTranslationMatrices.size();

	//		pNodeAnim->mPositionKeys = new aiVectorKey [numOfKeys];
	//		pNodeAnim->mRotationKeys = new aiQuatKey [numOfKeys];
	//		pNodeAnim->mScalingKeys = new aiVectorKey [numOfKeys];

	//		for (i32 k = 0; k < numOfKeys; k++)
	//		{
	//			// The position vector
	//			glm::vec3 translation = glm::vec3(meshScene.allNodes[i]->nodeAnimTranslationMatrices[i][k][3]);
	//			pNodeAnim->mPositionKeys[k].mValue = aiVector3D(translation.x, translation.y, translation.z);

	//			// The rotation matrix
	//			aiQuaternion rotation;
	//			glm::quat quaternion = glm::quat(glm::vec3(meshScene.allNodes[i]->eulerAngleXYZ[i][k]));
	//			rotation.w = quaternion.w;
	//			rotation.x = quaternion.x;
	//			rotation.y = quaternion.y;
	//			rotation.z = quaternion.z;
	//			pNodeAnim->mRotationKeys[k].mValue = rotation;

	//			// The scaling vector
	//			glm::vec3 scale(1);
	//			scale.x = meshScene.allNodes[i]->nodeAnimScalingMatrices[i][k][0][0];
	//			scale.y = meshScene.allNodes[i]->nodeAnimScalingMatrices[i][k][1][1];
	//			scale.z = meshScene.allNodes[i]->nodeAnimScalingMatrices[i][k][2][2];

	//		}
	//		pNodeAnim->mNumPositionKeys = meshScene.allNodes[i]->nodeAnimTranslationMatrices.size();
	//		pNodeAnim->mNumRotationKeys = meshScene.allNodes[i]->nodeAnimRotationMatrices.size();
	//		pNodeAnim->mNumScalingKeys = meshScene.allNodes[i]->nodeAnimScalingMatrices.size();
	//	}
	//}

	return scene;

}

void NoxEngine::exportFBX(aiScene *scene)
{
	Assimp::Exporter exporter;
	std::string file_path = "assets/hahaha.fbx";
	if (exporter.Export(scene, "fbx", file_path) != AI_SUCCESS)
		printf("%s\n", exporter.GetErrorString());
	delete scene;
}

