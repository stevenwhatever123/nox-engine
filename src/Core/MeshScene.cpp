#include <Core/MeshScene.h>
#include <Core/Mesh.h>

#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <Utils/Utils.h>


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
	MeshNode* rootNode = new MeshNode;
	aiNode* aiRootNode = scene->mRootNode;

	// We create the node hierarchy first than extract animation data into the mesh nodes
	createNodeHierarchy(aiRootNode, rootNode);

	// Animation Data
	if (scene->mNumAnimations > 0) {
		extractAnimationInfo(scene);
	}
}

MeshScene::MeshScene(std::istream& stream)
{
	stream.read((char*)&nodeHierarchy, sizeof(MeshNode));

	size_t allNodeSize = allNodes.size();
	stream.read((char*)&allNodeSize, sizeof(allNodeSize));
	allNodes.resize(allNodeSize);
	for (u32 i = 0; i < allNodeSize; i++)
	{
		allNodes[i] = new MeshNode();
		allNodes[i]->deserialize(stream);
	}

	size_t meshSize = allNodes.size();
	stream.read((char*)&meshSize, sizeof(meshSize));
	meshes.resize(meshSize);
	for (u32 i = 0; i < meshSize; i++)
	{
		meshes[i] = new Mesh();
		meshes[i]->deserialize(stream);
	}

	stream.read((char*)&frameIndex, sizeof(u32));
	stream.read((char*)&animationIndex, sizeof(u32));

	stream.read((char*)&accumulator, sizeof(time_type));
	stream.read((char*)&timeStep, sizeof(time_type));

	stream.read((char*)&whichTickFloor, sizeof(i32));
	stream.read((char*)&whichTickCeil, sizeof(i32));

	size_t animationSize = animations.size();
	stream.read((char*)&animationSize, sizeof(animationSize));
	animations.resize(animationSize);
	stream.read((char*)&animations[0], animationSize * sizeof(aiAnimation));

	size_t numberOfTicks;
	stream.read((char*)&numberOfTicks, sizeof(numberOfTicks));
	numTicks.resize(numberOfTicks);
	stream.read((char*)&numTicks[0], numberOfTicks * sizeof(i32));

	animationDuration.resize(animationSize);
	stream.read((char*)&animationDuration[0], animationSize * sizeof(time_type));

	stream.read((char*)&playAnimation, sizeof(playAnimation));
}

// Some function to print out data just to make sure we're getting data correctly
void MeshScene::printAllNodes()
{
	for (MeshNode *node : allNodes)
	{
		LOG_DEBUG("Node Name: %s", node->name.c_str());
	}
}

// Some function to print out data just to make sure we're getting data correctly
void MeshScene::printAllMeshNodes()
{
	for (MeshNode *node : allNodes)
	{
		if (node->meshIndex.size() > 0)
		{
			LOG_DEBUG("Name: %s", node->name.c_str());
			LOG_DEBUG("Associated MeshIndex: %s", node->name.c_str());
			for (u32 meshIndex : node->meshIndex) { LOG_DEBUG("\t%d", meshIndex); }
			LOG_DEBUG("Transform Mat:\n%s",  glm::to_string(node->transformation).c_str());
		}
	}
}

void MeshScene::loopAllNodes(MeshNode node, std::vector<MeshNode>& list)
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
	if (hasAnimations())
	{
		u32 numOfTicks = numTicks[animationIndex];

		if ((accumulator / timeStep) > 0)
		{
			whichTickFloor = frameIndex;
			if (frameIndex < (numOfTicks - 1))
			{
				whichTickCeil = frameIndex + 1;
			}
			else
			{
				whichTickCeil = frameIndex;
			}
		}
	}
}

// Some modelling software handle UV coordinates differently
// Flip it if necessary
void MeshScene::flipUV()
{
	for (Mesh* mesh : meshes)
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

	if (num < 0)
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

u32 MeshScene::getNumOfAnimations() const
{
	//return (u32)animations.size();
	return (u32)numTicks.size();
}

bool MeshScene::hasAnimations()
{
	return getNumOfAnimations() > 0;
}

void MeshScene::updateNumTicks(u32 animationIndex, u32 num)
{
	// Don't do anything if the number is the same
	if (num == numTicks[animationIndex])
		return;

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

void MeshScene::insertFrame(u32 animationIndex, u32 selectedFrame)
{
	if (hasAnimations())
	{
		numTicks[animationIndex] = numTicks[animationIndex] + 1;
	}
	else
	{
		numTicks.resize(animationIndex, 0);
		numTicks[animationIndex] = 1;
	}

	for (u32 i = 0; i < allNodes.size(); i++)
	{
		allNodes[i]->insertFrameAfter(animationIndex, selectedFrame);
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

void MeshScene::createNodeHierarchy(aiNode* aiRootnode, MeshNode* rootNode)
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
	transformation[0][0] = (f32)aiRootnode->mTransformation.a1;
	transformation[0][1] = (f32)aiRootnode->mTransformation.b1;
	transformation[0][2] = (f32)aiRootnode->mTransformation.c1;
	transformation[0][3] = (f32)aiRootnode->mTransformation.d1;

	transformation[1][0] = (f32)aiRootnode->mTransformation.a2;
	transformation[1][1] = (f32)aiRootnode->mTransformation.b2;
	transformation[1][2] = (f32)aiRootnode->mTransformation.c2;
	transformation[1][3] = (f32)aiRootnode->mTransformation.d2;

	transformation[2][0] = (f32)aiRootnode->mTransformation.a3;
	transformation[2][1] = (f32)aiRootnode->mTransformation.b3;
	transformation[2][2] = (f32)aiRootnode->mTransformation.c3;
	transformation[2][3] = (f32)aiRootnode->mTransformation.d3;

	transformation[3][0] = (f32)aiRootnode->mTransformation.a4;
	transformation[3][1] = (f32)aiRootnode->mTransformation.b4;
	transformation[3][2] = (f32)aiRootnode->mTransformation.c4;
	transformation[3][3] = (f32)aiRootnode->mTransformation.d4;
	// =====================================================

	rootNode->transformation = transformation;

	allNodes.push_back(rootNode);

	traverseTreeStructure(aiRootnode, rootNode);
	nodeHierarchy = *rootNode;
}

void MeshScene::traverseTreeStructure(aiNode* node, MeshNode* targetParent)
{
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		MeshNode* child = new MeshNode;

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
		transformation[0][0] = (f32)node->mTransformation.a1;
		transformation[0][1] = (f32)node->mTransformation.a2;
		transformation[0][2] = (f32)node->mTransformation.a3;
		transformation[0][3] = (f32)node->mTransformation.a4;

		transformation[1][0] = (f32)node->mTransformation.b1;
		transformation[1][1] = (f32)node->mTransformation.b2;
		transformation[1][2] = (f32)node->mTransformation.b3;
		transformation[1][3] = (f32)node->mTransformation.b4;

		transformation[2][0] = (f32)node->mTransformation.c1;
		transformation[2][1] = (f32)node->mTransformation.c2;
		transformation[2][2] = (f32)node->mTransformation.c3;
		transformation[2][3] = (f32)node->mTransformation.c4;

		transformation[3][0] = (f32)node->mTransformation.d1;
		transformation[3][1] = (f32)node->mTransformation.d2;
		transformation[3][2] = (f32)node->mTransformation.d3;
		transformation[3][3] = (f32)node->mTransformation.d4;
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

		Mesh* mesh = new Mesh();

		mesh->name = pMesh->mName.C_Str();
		mesh->hasBones = pMesh->HasBones();

		// Fix this too
		mesh->vertices.resize(pMesh->mNumVertices);
		mesh->normals.resize(pMesh->mNumVertices);
		mesh->faces.resize(pMesh->mNumFaces);
		mesh->texCoords.resize(pMesh->mNumVertices);

		const bool has_texture = pMesh->HasTextureCoords(0);

		const aiVector3D* pVertex = pMesh->mVertices;
		const aiVector3D* pNormal = pMesh->mNormals;
		const aiVector3D* pTexCoord = has_texture ? pMesh->mTextureCoords[0] : &zero3D;

		for (u32 j = 0; j < pMesh->mNumVertices; ++j) {

			mesh->vertices[j].x = (f32)pVertex->x;
			mesh->vertices[j].y = (f32)pVertex->y;
			mesh->vertices[j].z = (f32)pVertex->z;

			mesh->normals[j].x = (f32)pNormal->x;
			mesh->normals[j].y = (f32)pNormal->y;
			mesh->normals[j].z = (f32)pNormal->z;

			mesh->texCoords[j].x = (f32)pTexCoord->x;
			mesh->texCoords[j].y = (f32)pTexCoord->y;

			pVertex++;
			pNormal++;
			if(has_texture) {
				pTexCoord++;
			}
		}

		const aiFace* face = pMesh->mFaces;
		for (u32 j = 0; j < pMesh->mNumFaces; ++j)
		{

			if (face->mNumIndices == 3)
			{
				mesh->faces[j][0] = (i32)(face->mIndices[0]);
				mesh->faces[j][1] = (i32)(face->mIndices[1]);
				mesh->faces[j][2] = (i32)(face->mIndices[2]);
			} else {
				Logger::debug("Error: number of face indicies is less than 3");
			}

			face++;
		}

		// mesh->prepTheData();
		meshes.push_back(mesh);

	}



	// aiMesh** loadedMesh = scene->mMeshes;

	// const aiVector3D zero3D(0.0f, 0.0f, 0.0f);

	// // Get vertex, normal and textcoord data
	// for (u32 i = 0; i < scene->mNumMeshes; ++i) {

	// 	const aiMesh* pMesh = loadedMesh[i];

	// 	Mesh2* mesh = new Mesh2;

	// 	mesh->hasBones = pMesh->HasBones();
	// 	mesh->name = pMesh->mName.C_Str();

	// 	const bool has_texture = pMesh->HasTextureCoords(0);

	// 	const aiVector3D* pVertex = pMesh->mVertices;
	// 	const aiVector3D* pNormal = pMesh->mNormals;
	// 	const aiVector3D* pTexCoord = has_texture ? pMesh->mTextureCoords[0] : &zero3D;

	// 	// Resizing and reserve spaces
	// 	mesh->vertices.resize(pMesh->mNumVertices);
	// 	mesh->normals.resize(pMesh->mNumVertices);
	// 	mesh->texCoords.resize(pMesh->mNumVertices);
	// 	mesh->faceIndices.resize(pMesh->mNumFaces * 3);

	// 	for (u32 j = 0; j < pMesh->mNumVertices; ++j) 
	// 	{
	// 		mesh->vertices[i][j].x = (f32)pVertex->x;
	// 		mesh->vertices[i][j].y = (f32)pVertex->y;
	// 		mesh->vertices[i][j].z = (f32)pVertex->z;

	// 		mesh->normals[i][j].x = (f32)pNormal->x;
	// 		mesh->normals[i][j].y = (f32)pNormal->y;
	// 		mesh->normals[i][j].z = (f32)pNormal->z;

	// 		mesh->texCoords[j].x = (f32)pTexCoord->x;
	// 		mesh->texCoords[j].y = (f32)pTexCoord->y;

	// 		pVertex++;
	// 		pNormal++;
	// 		if (has_texture) {
	// 			pTexCoord++;
	// 		}
	// 	}

	// 	const aiFace* face = pMesh->mFaces;
	// 	for (u32 j = 0; j < pMesh->mNumFaces; ++j)
	// 	{

	// 		if (face->mNumIndices == 3)
	// 		{
	// 			mesh->faceIndices[i][j][0] = face->mIndices[0];
	// 			mesh->faceIndices[i][j][1] = face->mIndices[1];
	// 			mesh->faceIndices[i][j][2] = face->mIndices[2];
	// 		}
	// 		else {
	// 			Logger::debug("Error: number of face indicies is less than 3");
	// 		}

	// 		face++;
	// 	}

			// }
}

void MeshScene::extractTextureInfo(const aiScene* scene, Mesh* mesh) {
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

			}
		}
	}

}

void MeshScene::extractAnimationInfo(const aiScene* scene) {

	// Animation Data
	u32 numAnimation = scene->mNumAnimations;

	// Resizing and reserving spaces
	animations.resize(numAnimation);

	numTicks.resize(numAnimation);
	animationDuration.resize(numAnimation);

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

			for (MeshNode* node : allNodes)
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
						rotationMatrix[0][0] = (f32)animation_chl->mRotationKeys[k].mValue.GetMatrix().a1;
						rotationMatrix[0][1] = (f32)animation_chl->mRotationKeys[k].mValue.GetMatrix().b1;
						rotationMatrix[0][2] = (f32)animation_chl->mRotationKeys[k].mValue.GetMatrix().c1;

						rotationMatrix[1][0] = (f32)animation_chl->mRotationKeys[k].mValue.GetMatrix().a2;
						rotationMatrix[1][1] = (f32)animation_chl->mRotationKeys[k].mValue.GetMatrix().b2;
						rotationMatrix[1][2] = (f32)animation_chl->mRotationKeys[k].mValue.GetMatrix().c2;

						rotationMatrix[2][0] = (f32)animation_chl->mRotationKeys[k].mValue.GetMatrix().a3;
						rotationMatrix[2][1] = (f32)animation_chl->mRotationKeys[k].mValue.GetMatrix().b3;
						rotationMatrix[2][2] = (f32)animation_chl->mRotationKeys[k].mValue.GetMatrix().c3;

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

void MeshScene::serialize(std::ostream& stream)
{
	stream.write((char*)&nodeHierarchy, sizeof(MeshNode));

	size_t allNodeSize = allNodes.size();
	stream.write((char*)&allNodeSize, sizeof(allNodeSize));
	for (u32 i = 0; i < allNodeSize; i++)
	{
		allNodes[i]->serialize(stream);
	}

	size_t meshSize = meshes.size();
	stream.write((char*)&meshSize, sizeof(meshSize));
	for (u32 i = 0; i < meshSize; i++)
	{
		meshes[i]->serialize(stream);
	}

	stream.write((char*)&frameIndex, sizeof(u32));
	stream.write((char*)&animationIndex, sizeof(u32));

	stream.write((char*)&accumulator, sizeof(time_type));
	stream.write((char*)&timeStep, sizeof(time_type));

	stream.write((char*)&whichTickFloor, sizeof(i32));
	stream.write((char*)&whichTickCeil, sizeof(i32));

	size_t animationSize = animations.size();
	stream.write((char*)&animationSize, sizeof(animationSize));
	stream.write((char*)&animations[0], animationSize * sizeof(aiAnimation));

	size_t numberOfTicks = numTicks.size();
	stream.write((char*)&numberOfTicks, sizeof(numberOfTicks));
	stream.write((char*)&numTicks[0], numberOfTicks * sizeof(i32));

	stream.write((char*)&animationDuration[0], animationSize * sizeof(time_type));

	stream.write((char*)&playAnimation, sizeof(playAnimation));
}
