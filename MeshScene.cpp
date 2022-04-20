#include <MeshScene.h>
#include <Mesh2.h>
#include <iostream>
#include <glm/gtx/string_cast.hpp>


using namespace NoxEngine;
using NoxEngineUtils::Logger;

MeshScene::MeshScene() { }
MeshScene::~MeshScene() { }

MeshScene::MeshScene(const aiScene* scene) :
	frameIndex(0),
	animationIndex(0),
	accumulator(0),
	timeStep(0),
	whichTickFloor(0),
	whichTickCeil(0)
{

	std::cout << "Number of meshes: " << scene->mNumMeshes << "\n";

	extractGeometricInfo(scene);

	//// extractTextureInfo(scene);

	//// Animation Data
	//if (scene->mNumAnimations > 0)
	//{
	//	extractAnimationInfo(scene);
	//}

	//// Node Hierarchy
	//MeshNode* rootNode = new MeshNode;
	//aiNode* aiRootNode = scene->mRootNode;

	//createNodeHierarchy(aiRootNode, rootNode);
}

void MeshScene::extractGeometricInfo(const aiScene* scene) {

	aiMesh** loadedMesh = scene->mMeshes;

	const aiVector3D zero3D(0.0f, 0.0f, 0.0f);

	// Get vertex, normal and textcoord data
	for (u32 i = 0; i < scene->mNumMeshes; ++i) {

		const aiMesh* pMesh = loadedMesh[i];

		Mesh2* mesh = new Mesh2;

		mesh->hasBones = pMesh->HasBones();

		const bool has_texture = pMesh->HasTextureCoords(0);

		const aiVector3D* pVertex = pMesh->mVertices;
		const aiVector3D* pNormal = pMesh->mNormals;
		const aiVector3D* pTexCoord = has_texture ? pMesh->mTextureCoords[0] : &zero3D;

		// Resizing and reserve spaces
		mesh->vertices.resize(pMesh->mNumVertices);
		mesh->normals.resize(pMesh->mNumVertices);
		mesh->texCoords.resize(pMesh->mNumVertices);
		mesh->faceIndices.resize(pMesh->mNumFaces * 3);

		for (u32 j = 0; j < pMesh->mNumVertices; ++j) 
		{
			mesh->vertices[j].x = pVertex->x;
			mesh->vertices[j].y = pVertex->y;
			mesh->vertices[j].z = pVertex->z;

			mesh->normals[j].x = pNormal->x;
			mesh->normals[j].y = pNormal->y;
			mesh->normals[j].z = pNormal->z;

			mesh->texCoords[j].x = pTexCoord->x;
			mesh->texCoords[j].y = pTexCoord->y;

			pVertex++;
			pNormal++;
			if (has_texture) {
				pTexCoord++;
			}
		}

		const aiFace* face = pMesh->mFaces;
		for (u32 j = 0; j < pMesh->mNumFaces; ++j)
		{

			if (face->mNumIndices == 3)
			{
				mesh->faceIndices.push_back(face->mIndices[0]);
				mesh->faceIndices.push_back(face->mIndices[1]);
				mesh->faceIndices.push_back(face->mIndices[2]);
			}
			else {
				Logger::debug("Error: number of face indicies is less than 3");
			}

			face++;
		}

		someFunctions(scene, mesh);
	}
}

void MeshScene::someFunctions(const aiScene* scene, Mesh2& mesh)
{
	std::cout << mesh.vertices.size() << "\n";
}