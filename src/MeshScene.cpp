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
	extractGeometricInfo(scene);
	
	// Node Hierarchy
	MeshNode2* rootNode = new MeshNode2;
	aiNode* aiRootNode = scene->mRootNode;

	// We create the node hierarchy first than extract animation data into the mesh nodes
	createNodeHierarchy(aiRootNode, rootNode);

	// Animation Data
	if (scene->mNumAnimations > 0)
	{
		extractAnimationInfo(scene);
	}
}

// Some function to print out data just to make sure we're getting data correctly
void MeshScene::printAllNodes()
{
	for (MeshNode2 *node : allNodes)
	{
		std::cout << node->name << "\n";
	}
}

// Some function to print out data just to make sure we're getting data correctly
void MeshScene::printAllMeshNodes()
{
	for (MeshNode2 *node : allNodes)
	{
		if (node->meshIndex.size() > 0)
		{
			std::cout << "Name: " << node->name << ": " << "\n";
			std::cout << "Associated meshIndex: ";
			for (u32 meshIndex : node->meshIndex)
			{
				std::cout << meshIndex << " ";
			}
			std::cout << "\n";
			std::cout << glm::to_string(node->transformation) << "\n";
			std::cout << "====================================================" << "\n";
		}
	}
}

void MeshScene::loopAllNodes(MeshNode2 node, std::vector<MeshNode2>& list)
{
	list.push_back(node);
	for (u32 i = 0; i < node.child.size(); ++i)
	{
		list.push_back(node.child[i]);

		loopAllNodes(node.child[i], list);
	}
}

// Frame based update function
void MeshScene::update()
{
	if (hasAnimations())
	{
		u32 numOfTicks = numTicks[animationIndex];
		if (frameIndex < numOfTicks - 1)
		{
			frameIndex++;
		}
	}
}

// Semi-fixed time based update function
void MeshScene::update(time_type dt)
{
	if (hasAnimations())
	{
		accumulator += dt;

		u32 numOfTicks = numTicks[animationIndex];
		if (frameIndex < numOfTicks - 1)
		{
			// Interpolate time into tick
			time_type duration = animationDuration[animationIndex];
			timeStep = duration / numOfTicks;

			while (accumulator >= timeStep)
			{
				if (frameIndex < numOfTicks - 1)
					frameIndex++;

				accumulator -= timeStep;
			}

			// Remaining accumulated time
			//const time_type alpha = accumulator / timeStep;
			//frameIndex = frameIndex * alpha + frameIndex * (1.0 - alpha);

			updateCeilAndFloor();
		}
	}
}

void MeshScene::updateCeilAndFloor()
{
	u32 numOfTicks = numTicks[animationIndex];

	whichTickFloor = frameIndex;
	whichTickCeil = frameIndex < numOfTicks - 1 ? whichTickFloor + 1 : frameIndex;
}

// Some modelling software handle UV coordinates differently
// Flip it if necessary
void MeshScene::flipUV()
{
	for (Mesh2* mesh : meshes)
	{
		for (u32 i = 0; i < mesh->texCoords.size(); i++)
		{
			mesh->texCoords[i].y = 1 - mesh->texCoords[i].y;
		}
	}
}

void MeshScene::setAnimationIndex(u32 num)
{
	if (num > getNumOfAnimations() - 1)
		return;

	animationIndex = num;
}

void MeshScene::resetAnimation()
{
	frameIndex = 0;
	accumulator = 0;
	timeStep = 0;
	whichTickCeil = 0;
	whichTickFloor = 0;
}

u32 MeshScene::getNumOfAnimations()
{
	return (u32)animations.size();
}

bool MeshScene::hasAnimations()
{
	return getNumOfAnimations() > 0;
}

void MeshScene::updateNumTicks(u32 animationIndex, u32 num)
{
	if (hasAnimations())
	{
		numTicks[animationIndex] = num;
	}
	else
	{
		numTicks.resize(animationIndex, 0);
		numTicks[animationIndex] = num;
	}

	for (u32 i = 0; i < allNodes.size(); i++)
	{
		allNodes[i]->updateAnimationSize(animationIndex, num);
	}
}

void MeshScene::setFrameIndex(u32 index)
{
	if (index == frameIndex || index < 0)
	{
		return;
	}

	frameIndex = index;
}

void MeshScene::createNodeHierarchy(aiNode* aiRootnode, MeshNode2* rootNode)
{
	rootNode->name = aiRootnode->mName.C_Str();
	rootNode->parent = nullptr;

	if (aiRootnode->mNumMeshes > 0)
	{
		// Apparently there can be more than one mesh index
		for (u32 i = 0; i < aiRootnode->mNumMeshes; i++)
		{
			rootNode->meshIndex.push_back(aiRootnode->mMeshes[i]);
		}
	}

	// =====================================================
	// Convert aiMatrix4x4 to glm::mat4
	// We can convert this to a tool later on
	glm::mat4 transformation;
	transformation[0][0] = aiRootnode->mTransformation.a1;
	transformation[0][1] = aiRootnode->mTransformation.b1;
	transformation[0][2] = aiRootnode->mTransformation.c1;
	transformation[0][3] = aiRootnode->mTransformation.d1;

	transformation[1][0] = aiRootnode->mTransformation.a2;
	transformation[1][1] = aiRootnode->mTransformation.b2;
	transformation[1][2] = aiRootnode->mTransformation.c2;
	transformation[1][3] = aiRootnode->mTransformation.d2;

	transformation[2][0] = aiRootnode->mTransformation.a3;
	transformation[2][1] = aiRootnode->mTransformation.b3;
	transformation[2][2] = aiRootnode->mTransformation.c3;
	transformation[2][3] = aiRootnode->mTransformation.d3;

	transformation[3][0] = aiRootnode->mTransformation.a4;
	transformation[3][1] = aiRootnode->mTransformation.b4;
	transformation[3][2] = aiRootnode->mTransformation.c4;
	transformation[3][3] = aiRootnode->mTransformation.d4;
	// =====================================================

	rootNode->transformation = transformation;

	allNodes.push_back(rootNode);

	traverseTreeStructure(aiRootnode, rootNode);
	nodeHierarchy = *rootNode;
}

void MeshScene::traverseTreeStructure(aiNode* node, MeshNode2* targetParent)
{
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		MeshNode2* child = new MeshNode2;

		child->name = node->mChildren[i]->mName.C_Str();
		child->parent = targetParent;

		if (node->mChildren[i]->mNumMeshes > 0)
		{
			// Apparently there can be more than one mesh index
			for (u32 j = 0; j < node->mChildren[i]->mNumMeshes; j++)
			{
				child->meshIndex.push_back(node->mChildren[i]->mMeshes[j]);
			}
		}

		// ==============================================
		// Convert aiMatrix4x4 to glm::mat4
		// We can convert this to a tool later on
		glm::mat4 transformation;
		transformation[0][0] = node->mTransformation.a1;
		transformation[0][1] = node->mTransformation.a2;
		transformation[0][2] = node->mTransformation.a3;
		transformation[0][3] = node->mTransformation.a4;

		transformation[1][0] = node->mTransformation.b1;
		transformation[1][1] = node->mTransformation.b2;
		transformation[1][2] = node->mTransformation.b3;
		transformation[1][3] = node->mTransformation.b4;

		transformation[2][0] = node->mTransformation.c1;
		transformation[2][1] = node->mTransformation.c2;
		transformation[2][2] = node->mTransformation.c3;
		transformation[2][3] = node->mTransformation.c4;

		transformation[3][0] = node->mTransformation.d1;
		transformation[3][1] = node->mTransformation.d2;
		transformation[3][2] = node->mTransformation.d3;
		transformation[3][3] = node->mTransformation.d4;
		// ==============================================

		child->transformation = transformation;

		allNodes.push_back(child);

		traverseTreeStructure(node->mChildren[i], child);
		targetParent->child.push_back(*child);
	}
}

void MeshScene::extractGeometricInfo(const aiScene* scene) {

	aiMesh** loadedMesh = scene->mMeshes;

	const aiVector3D zero3D(0.0f, 0.0f, 0.0f);

	// Get vertex, normal and textcoord data
	for (u32 i = 0; i < scene->mNumMeshes; ++i) {

		const aiMesh* pMesh = loadedMesh[i];

		Mesh2* mesh = new Mesh2;

		mesh->hasBones = pMesh->HasBones();
		mesh->name = pMesh->mName.C_Str();

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

		mesh->prepTheData();

		meshes.push_back(mesh);
	}
}

void MeshScene::extractTextureInfo(const aiScene* scene, Mesh2* mesh) {
	// I say we can implement it later
	// Initialise Materials and textures
	for (u32 i = 0; i < scene->mNumMaterials; ++i)
	{
		const aiMaterial* pMaterial = scene->mMaterials[i];
		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString path;
			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			{
				//std::string fullPath = path.data;
				//std::cout << fullPath << "\n";
			}
		}
	}

}

void MeshScene::extractAnimationInfo(const aiScene* scene) {

	// Animation Data
	u32 numAnimation = scene->mNumAnimations;

	// Resizing and reserving spaces
	animations.resize(numAnimation);

	// We probably don't want to resize it since not every node has animation data
	//for (MeshNode2* node : allNodes)
	//{
	//	node->nodeAnimations.resize(numAnimation);
	//	node->nodeAnimTransformation.resize(numAnimation);
	//	node->nodeAnimTranslationMatrices.resize(numAnimation);
	//	node->nodeAnimRotationMatrices.resize(numAnimation);
	//	node->nodeAnimScalingMatrices.resize(numAnimation);
	//}

	numTicks.resize(numAnimation);
	animationDuration.resize(numAnimation);

	// Loop through every animation data/clip
	for (u32 i = 0; i < numAnimation; i++)
	{
		//NOTE(sharo): we are keeping pointers to the insides of the scene around
		//but not the scene itself

		animations[i] = scene->mAnimations[i];

		u32 numChannels = scene->mAnimations[i]->mNumChannels;

		aiAnimation* scene_animation = scene->mAnimations[i];

		// Loop through every node
		for (u32 j = 0; j < numChannels; j++)
		{
			// Channels means the nodes that would be moving
			// Note that not every nodes would be animated
			// So the number of channels may be smaller than the number of 
			// nodes

			// We will pair animation channels with node name
			// But we're ignore it at this moment

			aiNodeAnim* animation_chl = scene_animation->mChannels[j];

			for (MeshNode2* node : allNodes)
			{
				// Pair the channel with node
				// We need to use strcmp if we're comparing const char*
				if (strcmp(node->name.c_str(), animation_chl->mNodeName.C_Str()) == 0)
				{
					node->nodeAnimations.push_back(animation_chl);
					// Resizing and reserving spaces
					node->nodeAnimTransformation.resize(numAnimation);
					node->nodeAnimTranslationMatrices.resize(numAnimation);
					node->nodeAnimRotationMatrices.resize(numAnimation);
					node->nodeAnimScalingMatrices.resize(numAnimation);

					node->eulerAngleXYZ.resize(numAnimation);
					node->maximumFrame.resize(numAnimation);
					
					// Retrieve transformation matrices from keyframes in ascending
					for (u32 k = 0; k < animation_chl->mNumPositionKeys; k++)
					{
						// Translation matrix
						glm::mat4 translationMatrix =
							glm::translate(
								glm::mat4(1),
								glm::vec3(
									animation_chl->mPositionKeys[k].mValue.x,
									animation_chl->mPositionKeys[k].mValue.y,
									animation_chl->mPositionKeys[k].mValue.z
								)
							);

						// Scaling matrix
						glm::mat4 scalingMatrix = glm::mat4(1.0f);
						scalingMatrix =
							glm::scale(
								scalingMatrix,
								glm::vec3(
									animation_chl->mScalingKeys[k].mValue.x,
									animation_chl->mScalingKeys[k].mValue.y,
									animation_chl->mScalingKeys[k].mValue.z
								)
							);

						// Rotation matrix
						// I think this is how it works, row based
						glm::mat4 rotationMatrix = glm::mat4(1.0f);
						rotationMatrix[0][0] = animation_chl->mRotationKeys[k].mValue.GetMatrix().a1;
						rotationMatrix[0][1] = animation_chl->mRotationKeys[k].mValue.GetMatrix().b1;
						rotationMatrix[0][2] = animation_chl->mRotationKeys[k].mValue.GetMatrix().c1;

						rotationMatrix[1][0] = animation_chl->mRotationKeys[k].mValue.GetMatrix().a2;
						rotationMatrix[1][1] = animation_chl->mRotationKeys[k].mValue.GetMatrix().b2;
						rotationMatrix[1][2] = animation_chl->mRotationKeys[k].mValue.GetMatrix().c2;

						rotationMatrix[2][0] = animation_chl->mRotationKeys[k].mValue.GetMatrix().a3;
						rotationMatrix[2][1] = animation_chl->mRotationKeys[k].mValue.GetMatrix().b3;
						rotationMatrix[2][2] = animation_chl->mRotationKeys[k].mValue.GetMatrix().c3;

						// Combine all of them to get the transformation matrix
						// Notes: I don't think it's a good idea to include translation
						// inside the animation data 
						// This would mean everthing the gameobject moves
						// is tied to the animation clip and may cause various 
						// problems based on my previous experience

						node->nodeAnimTranslationMatrices[i].push_back(translationMatrix);
						node->nodeAnimRotationMatrices[i].push_back(rotationMatrix);
						node->nodeAnimScalingMatrices[i].push_back(scalingMatrix);
						node->nodeAnimTransformation[i].push_back(translationMatrix * rotationMatrix * scalingMatrix);
						node->setupEulerAngle();
					}

					node->updateMaximumFrame(i, animation_chl->mNumPositionKeys);

					break;
				} // Key frames
			} // Matched node
		} // j loop - channels

		// pre generate data for numer of ticks
		numTicks[i] = scene_animation->mChannels[0]->mNumPositionKeys;
		// Pre generate for animation duration
		animationDuration[i] = numTicks[i] / scene_animation->mTicksPerSecond;
	}

}
