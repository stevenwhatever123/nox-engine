#include "FBXFileLoader.h"
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/gtx/string_cast.hpp>

#include "MeshNode.h"
#include "Mesh.h"

namespace FBXFileLoader
{
	const aiScene* readFBX(const char* filename)
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

			printf("Success reading '%s'\n", filename);

			//std::cout << pScene->mNumMeshes << "\n";

			return pScene;
		}
		else
		{
			printf("Error parsing '%s': '%s'\n", filename,
												 importer.GetErrorString());

			return nullptr;
		}
	}

	Mesh * getMesh(const aiScene* pScene)
	{
		// ==========================================================
		// Convert to mesh
		Mesh * mesh = new Mesh();

		aiMesh** loadedMesh = pScene->mMeshes;

		mesh->resizeNumOfMeshes(pScene->mNumMeshes);

		// Get vertex, normal and textcoord data
		for (unsigned int i = 0; i < pScene->mNumMeshes; ++i)
		{
			const aiMesh* pMesh = loadedMesh[i];

			std::vector<glm::vec3> mVertices;
			std::vector<glm::vec3> mNormals;
			std::vector<unsigned int> mIndices;
			std::vector<glm::vec2> mTexCoord;

			const aiVector3D zero3D(0.0f, 0.0f, 0.0f);

			for (unsigned int j = 0; j < pMesh->mNumVertices; ++j)
			{
				const aiVector3D* pVertex = &(pMesh->mVertices[j]);
				const aiVector3D* pNormal = &(pMesh->mNormals[j]);
				const aiVector3D* pTexCoord = pMesh->HasTextureCoords(0) ?
					&(pMesh->mTextureCoords[0][j]) : &zero3D;

				float vx = pVertex->x;
				float vy = pVertex->y;
				float vz = pVertex->z;

				float nx = pNormal->x;
				float ny = pNormal->y;
				float nz = pNormal->z;

				float u = pTexCoord->x;
				float v = pTexCoord->y;

				glm::vec3 vertex(vx, vy, vz);
				glm::vec3 normal(nx, ny, nz);
				glm::vec2 texCoord(u, v);

				mVertices.push_back(vertex);
				mNormals.push_back(normal);
				mTexCoord.push_back(texCoord);
			}

			for (unsigned int j = 0; j < pMesh->mNumFaces; ++j)
			{
				const aiFace& face = pMesh->mFaces[j];

				if (face.mNumIndices == 3)
				{
					mIndices.push_back(face.mIndices[0]);
					mIndices.push_back(face.mIndices[1]);
					mIndices.push_back(face.mIndices[2]);
				}
				else
				{
					printf("Error: number of face indicies is less than 3");
				}
			}

			mesh->vertices[i] = mVertices;
			mesh->normals[i] = mNormals;
			mesh->texCoord[i] = mTexCoord;
			mesh->faceIndices[i] = mIndices;
		}

		// I say we can implement it later
		// Initialise Materials and textures
		for (unsigned int i = 0; i < pScene->mNumMaterials; ++i)
		{
			const aiMaterial* pMaterial = pScene->mMaterials[i];
			if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
			{
				aiString path;
				if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0,
					&path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
				{
					//std::string fullPath = path.data;
					//std::cout << fullPath << "\n";
				}
			}
		}

		std::cout << "Number of animations: " << pScene->mNumAnimations << "\n";

		// Animation Data
		if (pScene->mNumAnimations > 0)
		{
			unsigned int numAnimation = pScene->mNumAnimations;
			mesh->resizeNumOfAnimations(numAnimation);

			// Loop through every animation data/clip
			for (unsigned int i = 0; i < numAnimation; i++)
			{
				mesh->animations.push_back(pScene->mAnimations[i]);

				// Resize
				mesh->nodeAnimTranslationMatrices[i].resize(pScene->mAnimations[i]->mNumChannels);

				// Loop through every node
				for (unsigned int j = 0; j < pScene->mAnimations[i]->mNumChannels; j++)
				{
					// Channels means the nodes that would be moving
					// Note that not every nodes would be animated
					// So the number of channels may be smaller than the number of 
					// nodes

					// We will pair animation channels with node name
					// But we're ignore it at this moment

					mesh->nodeAnimations[i].push_back(
						pScene->mAnimations[i]->mChannels[j]
					);

					// Resize
					mesh->nodeAnimTransformation[i].resize(
						pScene->mAnimations[i]->mNumChannels
					);
					mesh->nodeAnimTranslationMatrices[i].resize(
						pScene->mAnimations[i]->mNumChannels
					);
					mesh->nodeAnimRotationMatrices[i].resize(
						pScene->mAnimations[i]->mNumChannels
					);
					mesh->nodeAnimScalingMatrices[i].resize(
						pScene->mAnimations[i]->mNumChannels
					);

					// Retrieve transformation matrices from keyframes in ascending
					// order
					for (unsigned int z = 0;
						z < pScene->mAnimations[i]->mChannels[j]->mNumPositionKeys;
						z++)
					{

						// Translation matrix
						glm::mat4 translationMatrix =
							glm::translate(
								glm::mat4(1),
								glm::vec3(
									pScene->mAnimations[i]
										->mChannels[j]->mPositionKeys[z].mValue.x,
									pScene->mAnimations[i]
										->mChannels[j]->mPositionKeys[z].mValue.y,
									pScene->mAnimations[i]
										->mChannels[j]->mPositionKeys[z].mValue.z
								)
							);

						// Scaling matrix
						glm::mat4 scalingMatrix = glm::mat4(1.0f);
						scalingMatrix =
							glm::scale(
								scalingMatrix,
								glm::vec3(
									pScene->mAnimations[i]
										->mChannels[j]->mScalingKeys[z].mValue.x,
									pScene->mAnimations[i]
										->mChannels[j]->mScalingKeys[z].mValue.y,
									pScene->mAnimations[i]
										->mChannels[j]->mScalingKeys[z].mValue.z
								)
							);

						// Rotation matrix
						// I think this is how it works, row based
						glm::mat4 rotationMatrix = glm::mat4(1.0f);
						rotationMatrix[0][0] = pScene->mAnimations[i]
							->mChannels[j]->mRotationKeys[z].mValue.GetMatrix().a1;
						rotationMatrix[0][1] = pScene->mAnimations[i]
							->mChannels[j]->mRotationKeys[z].mValue.GetMatrix().b1;
						rotationMatrix[0][2] = pScene->mAnimations[i]
							->mChannels[j]->mRotationKeys[z].mValue.GetMatrix().c1;

						rotationMatrix[1][0] = pScene->mAnimations[i]
							->mChannels[j]->mRotationKeys[z].mValue.GetMatrix().a2;
						rotationMatrix[1][1] = pScene->mAnimations[i]
							->mChannels[j]->mRotationKeys[z].mValue.GetMatrix().b2;
						rotationMatrix[1][2] = pScene->mAnimations[i]
							->mChannels[j]->mRotationKeys[z].mValue.GetMatrix().c2;

						rotationMatrix[2][0] = pScene->mAnimations[i]
							->mChannels[j]->mRotationKeys[z].mValue.GetMatrix().a3;
						rotationMatrix[2][1] = pScene->mAnimations[i]
							->mChannels[j]->mRotationKeys[z].mValue.GetMatrix().b3;
						rotationMatrix[2][2] = pScene->mAnimations[i]
							->mChannels[j]->mRotationKeys[z].mValue.GetMatrix().c3;

						// Combine all of them to get the transformation matrix
						// Notes: I don't think it's a good idea to include translation
						// inside the animation data 
						// This would mean everthing the gameobject moves
						// is tied to the animation clip and is not user defined
						// 
						// What we normally want to do is to animate the game
						// object rotations only and we define how the gameobject
						// moves via scripts/our tools

						//mesh->nodeAnimTranslationMatrices[i].push_back(
						//	translationMatrix
						//);
						mesh->nodeAnimTranslationMatrices[i][j].push_back(
							translationMatrix
						);

						mesh->nodeAnimRotationMatrices[i][j].push_back(
							rotationMatrix
						);

						mesh->nodeAnimScalingMatrices[i][j].push_back(
							scalingMatrix
						);

						glm::mat4 transformationMatrix =
							translationMatrix * rotationMatrix * scalingMatrix;

						mesh->nodeAnimTransformation[i][j].push_back(
							transformationMatrix
						);

					}
				}

				// Printing it out just to make sure everything is correct
				//for (unsigned int j = 0; j < pScene->mAnimations[i]->mNumChannels; j++)
				//{
				//	std::cout << mesh->nodeAnimations[i][j]->mNodeName.C_Str() << "\n";
				//	std::cout << "Number of key frames: "
				//		<< mesh->nodeAnimations[i][j]->mNumPositionKeys << "\n";;
				//}
			}
		}

		// Node Hierarchy
		MeshNode* rootNode = new MeshNode;
		aiNode* aiRootNode = pScene->mRootNode;
		mesh->createNodeHierarchy(aiRootNode, rootNode);

		// Apply transformation from the first animation as the idle transformation
		//if (pScene->mNumAnimations > 0)
		//{
		//	unsigned int index = 0;

		//	for (unsigned int i = 0; i < mesh->nodeAnimations[index].size(); i++)
		//	{
		//		// Loop through every node by name
		//		for (unsigned int j = 0; j < mesh->allNodes.size(); j++)
		//		{
		//			if (mesh->allNodes[j]->name ==
		//				mesh->nodeAnimations[index][i]->mNodeName.C_Str())
		//			{
		//				std::cout << glm::to_string(
		//					mesh->nodeAnimRotationMatrices[index][i]
		//				) << "\n";
		//			}
		//		}
		//	}
		//}

		//for (unsigned int i = 0; i < mesh->nodeAnimTranslationMatrices[0][0].size(); i++)
		//{
		//	std::cout << glm::to_string(mesh->nodeAnimRotationMatrices[0][0][i])
		//		<< "\n";
		//}

		// Apply transformation to all vertices from nodes


		// Print out all nodes
		//mesh.printAllNodes();
		//std::cout << "Mesh index " << mesh.nodeHierarchy.meshIndex<< "\n";

		//std::cout << rootNode->child[0].child[0].name << "\n";
		//std::cout << glm::to_string(rootNode->child[0].child[0].transformation) 
			//<< "\n";

		//for (unsigned int i = 0; i < mesh->nodeAnimRotationMatrices.size(); i++)
		//{
		//	for (unsigned int j = 0; j < mesh->nodeAnimRotationMatrices[i].size(); j++)
		//	{
		//		std::cout << glm::to_string(mesh->nodeAnimRotationMatrices[i][j]) 
		//			<< "\n";
		//	}
		//}

		// Optional: Print out all the data
		//unsigned int count = 0;
		//for (unsigned int i = 0; i < mesh->vertices.size(); i++)
		//{
		//	for (unsigned int j = 0; j < mesh->vertices[i].size(); j++)
		//	{
		//		std::cout << "Vertex " << count << ": " << mesh->vertices[i][j].x <<
		//			" " << mesh->vertices[i][j].y << " "
		//			<< mesh->vertices[i][j].z << "\n";
		//		count++;
		//	}
		//}

		//count = 0;
		//for (unsigned int i = 0; i < mesh->normals.size(); i++)
		//{
		//	for (unsigned int j = 0; j < mesh->normals[i].size(); j++)
		//	{
		//		std::cout << "Normal " << count << ": " << mesh->normals[i][j].x <<
		//			" " << mesh->normals[i][j].y << " "
		//			<< mesh->normals[i][j].z << "\n";
		//		count++;
		//	}
		//}

		//count = 0;
		//for (unsigned int i = 0; i < mesh->texCoord.size(); i++)
		//{
		//	for (unsigned int j = 0; j < mesh->texCoord[i].size(); j++)
		//	{
		//		std::cout << "TexCoord " << count << ": " << mesh->texCoord[i][j].x <<
		//			" " << mesh->texCoord[i][j].y << "\n";
		//		count++;
		//	}
		//}

		return mesh;
	}
}