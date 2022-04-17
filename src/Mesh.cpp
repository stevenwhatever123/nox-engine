#include "Mesh.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

using namespace NoxEngine;

Mesh::Mesh(const aiScene *scene) {
	aiMesh** loadedMesh = scene->mMeshes;

	vertices.resize(scene->mNumMeshes);
	normals.resize(scene->mNumMeshes);
	faceIndices.resize(scene->mNumMeshes);
	texCoord.resize(scene->mNumMeshes);

	const aiVector3D zero3D(0.0f, 0.0f, 0.0f);
	
	// Get vertex, normal and textcoord data
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {

		const aiMesh* pMesh = loadedMesh[i];

		vertices[i].resize(pMesh->mNumVertices);
		normals[i].resize(pMesh->mNumVertices);
		texCoord[i].resize(pMesh->mNumVertices);
		faceIndices[i].resize(pMesh->mNumFaces*3);
		

		const aiVector3D* pVertex = pMesh->mVertices;
		const aiVector3D* pNormal = pMesh->mNormals;
		const aiVector3D* pTexCoord = pMesh->HasTextureCoords(0) ?  pMesh->mTextureCoords[0] : &zero3D;

		const bool has_texture = pMesh->HasTextureCoords(0);

		for (u32 j = 0; j < pMesh->mNumVertices; ++j) {
			vertices[i].push_back(glm::vec3{pVertex->x, pVertex->y, pVertex->z});
			normals[i].push_back  (glm::vec3{pNormal->x, pNormal->y, pNormal->z});
			texCoord[i].push_back (glm::vec2{pVertex->x, pVertex->y});

			pVertex++;
			pNormal++;
			if(has_texture) {
				pTexCoord++;
			}
		}

		const aiFace* face = pMesh->mFaces;
		for (unsigned int j = 0; j < pMesh->mNumFaces; ++j)
		{

			if (face->mNumIndices == 3)
			{
				faceIndices[i].push_back(face->mIndices[0]);
				faceIndices[i].push_back(face->mIndices[1]);
				faceIndices[i].push_back(face->mIndices[2]);
			}
			else
			{
				printf("Error: number of face indicies is less than 3");
			}

			face++;
		}
	}

	// I say we can implement it later
	// Initialise Materials and textures
	for (u32 i = 0; i < scene->mNumMaterials; ++i)
	{
		const aiMaterial* pMaterial = scene->mMaterials[i];
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

	// Animation Data
	if (scene->mNumAnimations > 0)
	{
		u32 numAnimation = scene->mNumAnimations;
		resizeNumOfAnimations(numAnimation);

		// Loop through every animation data/clip
		for (u32 i = 0; i < numAnimation; i++)
		{
			animations.push_back(scene->mAnimations[i]);

			// Loop through every node
			for (u32 j = 0; j < scene->mAnimations[i]->mNumChannels; j++)
			{
				// Channels means the nodes that would be moving
				// Note that not every nodes would be animated
				// So the number of channels may be smaller than the number of 
				// nodes

				// We will pair animation channels with node name
				// But we're ignore it at this moment

				nodeAnimations[i].push_back( scene->mAnimations[i]->mChannels[j]);

				// Retrieve animation data to transformation matrices
				for (u32 z = 0; z < scene->mAnimations[i]->mChannels[j]->mNumPositionKeys; z++) {

					// Translation matrix
					glm::mat4 translationMatrix =
						glm::translate(
								glm::mat4(),
								glm::vec3(
									scene->mAnimations[i]->mChannels[j]->mPositionKeys[z].mValue.x,
									scene->mAnimations[i]->mChannels[j]->mPositionKeys[z].mValue.y,
									scene->mAnimations[i]->mChannels[j]->mPositionKeys[z].mValue.z
									)
								);

					// Scaling matrix
					glm::mat4 scalingMatrix = glm::mat4(1.0f);
					scalingMatrix =
						glm::scale(
								scalingMatrix,
								glm::vec3(
									scene->mAnimations[i]->mChannels[j]->mScalingKeys[z].mValue.x,
									scene->mAnimations[i]->mChannels[j]->mScalingKeys[z].mValue.y,
									scene->mAnimations[i]->mChannels[j]->mScalingKeys[z].mValue.z
									)
								);

					// Rotation matrix
					// I think this is how it works, row based
					// Note(sharo): you think? 
					glm::mat4 rotationMatrix = glm::mat4(1.0f);
					rotationMatrix[0][0] = scene->mAnimations[i]->mChannels[j]->mRotationKeys->mValue.GetMatrix().a1;
					rotationMatrix[0][1] = scene->mAnimations[i]->mChannels[j]->mRotationKeys->mValue.GetMatrix().a2;
					rotationMatrix[0][2] = scene->mAnimations[i]->mChannels[j]->mRotationKeys->mValue.GetMatrix().a3;

					rotationMatrix[1][0] = scene->mAnimations[i]->mChannels[j]->mRotationKeys->mValue.GetMatrix().b1;
					rotationMatrix[1][1] = scene->mAnimations[i]->mChannels[j]->mRotationKeys->mValue.GetMatrix().b2;
					rotationMatrix[1][2] = scene->mAnimations[i]->mChannels[j]->mRotationKeys->mValue.GetMatrix().b3;

					rotationMatrix[2][0] = scene->mAnimations[i]->mChannels[j]->mRotationKeys->mValue.GetMatrix().c1;
					rotationMatrix[2][1] = scene->mAnimations[i]->mChannels[j]->mRotationKeys->mValue.GetMatrix().c2;
					rotationMatrix[2][2] = scene->mAnimations[i]->mChannels[j]->mRotationKeys->mValue.GetMatrix().c3;

					// Combine all of them to get the transformation matrix
					// Notes: I don't think it's a good idea to include translation
					// inside the animation data 
					// This would mean everthing the gameobject moves
					// is tied to the animation clip and is not user defined
					// 
					// What we normally want to do is to animate the game
					// object rotations only and we define how the gameobject
					// moves via scripts/our tools

					nodeAnimTranslationMatrices[i].push_back( translationMatrix);
					nodeAnimRotationMatrices[i].push_back( rotationMatrix);
					nodeAnimScalingMatrices[i].push_back( scalingMatrix);
				}
			}
		}
	}

	// Node Hierarchy
	MeshNode* rootNode = new MeshNode;
	aiNode* aiRootNode = scene->mRootNode;
	createNodeHierarchy(aiRootNode, rootNode);

}

Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

void Mesh::resizeNumOfMeshes(unsigned int i)
{
	vertices.resize(i);
	normals.resize(i);
	faceIndices.resize(i);
	texCoord.resize(i);
}

void Mesh::resizeNumOfAnimations(unsigned int i)
{
	animations.resize(i);
	nodeAnimations.resize(i);
	nodeAnimTransformation.resize(i);
	nodeAnimTranslationMatrices.resize(i);
	nodeAnimRotationMatrices.resize(i);
	nodeAnimScalingMatrices.resize(i);

	numTicks.resize(i);
	animationDuration.resize(i);
}

void Mesh::createNodeHierarchy(aiNode* aiRootnode, MeshNode* rootNode)
{
	rootNode->name = aiRootnode->mName.C_Str();
	rootNode->parent = nullptr;

	// =====================================================
	// Convert aiMatrix4x4 to glm::mat4
	// We can convert this to a tool later on
	glm::mat4 transformation;
	transformation[0][0] = aiRootnode->mTransformation.a1;
	transformation[0][1] = aiRootnode->mTransformation.a2;
	transformation[0][2] = aiRootnode->mTransformation.a3;
	transformation[0][3] = aiRootnode->mTransformation.a4;

	transformation[1][0] = aiRootnode->mTransformation.b1;
	transformation[1][1] = aiRootnode->mTransformation.b2;
	transformation[1][2] = aiRootnode->mTransformation.b3;
	transformation[1][3] = aiRootnode->mTransformation.b4;

	transformation[2][0] = aiRootnode->mTransformation.c1;
	transformation[2][1] = aiRootnode->mTransformation.c2;
	transformation[2][2] = aiRootnode->mTransformation.c3;
	transformation[2][3] = aiRootnode->mTransformation.c4;

	transformation[3][0] = aiRootnode->mTransformation.d1;
	transformation[3][1] = aiRootnode->mTransformation.d2;
	transformation[3][2] = aiRootnode->mTransformation.d3;
	transformation[3][3] = aiRootnode->mTransformation.d4;
	// =====================================================

	rootNode->transformation = transformation;
	
	allNodes.push_back(rootNode);

	traverseTreeStructure(aiRootnode, rootNode);
	nodeHierarchy = *rootNode;
}

void Mesh::traverseTreeStructure(aiNode* node, MeshNode* targetParent)
{
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		MeshNode* child = new MeshNode;

		child->name = node->mChildren[i]->mName.C_Str();
		child->parent = targetParent;
		
		if (node->mChildren[i]->mNumMeshes > 0)
		{
			child->hasMesh = true;
			child->meshIndex = node->mChildren[i]->mMeshes[0];
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

// Some function to print out data just to make sure we're getting data correctly
void Mesh::printAllNodes()
{
	std::vector<MeshNode> list;

	loopAllNodes(nodeHierarchy, list);

	for (MeshNode node : list)
	{
		std::cout << node.name << "\n";
	}
}

// Some function to print out data just to make sure we're getting data correctly
void Mesh::printAllMeshNodes()
{
	std::vector<MeshNode> list;

	loopAllNodes(nodeHierarchy, list);

	for (MeshNode node : list)
	{
		if (node.hasMesh)
		{
			std::cout << node.name << ": " << "\n";
			std::cout << glm::to_string(node.transformation) << "\n";
			std::cout << "====================================================" << "\n";
		}
	}
}

void Mesh::loopAllNodes(MeshNode node, std::vector<MeshNode>& list)
{
	list.push_back(node);
	for (unsigned int i = 0; i < node.child.size(); ++i)
	{
		list.push_back(node.child[i]);

		loopAllNodes(node.child[i], list);
	}
}

// Frame based update function
void Mesh::update()
{
	if (nodeAnimTransformation.size() > 0)
	{
		if (frameIndex < nodeAnimTransformation[animationIndex][0].size() - 1)
		{
			frameIndex++;
		}
	}
}

// Semi-fixed time based update function
void Mesh::update(time_type dt)
{
	if (getNumOfAnimations() > 0)
	{
		accumulator += dt;

		if (frameIndex < nodeAnimTransformation[animationIndex][0].size() - 1)
		{
			// Interpolate time into tick
			int numOfTicks = numTicks[animationIndex];
			time_type duration = animationDuration[animationIndex];

			timeStep = duration / numOfTicks;

			while (accumulator >= timeStep)
			{
				if(frameIndex < numOfTicks - 1)
					frameIndex++;

				accumulator -= timeStep;
			}

			// Remaining accumulated time
			//const time_type alpha = accumulator / timeStep;
			//frameIndex = frameIndex * alpha + frameIndex * (1.0 - alpha);

			whichTickFloor = frameIndex;
			whichTickCeil = frameIndex < numOfTicks - 1? whichTickFloor + 1: frameIndex;
		}
	}
}

void Mesh::calculateCenterPosition()
{
	glm::vec3 temp(0,0,0);
	unsigned int count = 0;

	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		for (unsigned int j = 0; j < vertices[i].size(); j++)
		{
			temp = temp + vertices[i][j];
			count++;
		}
	}

	centerPosition = temp * (float) (1 / count);
}

// Just some testing function
void Mesh::generateAnimation(glm::vec3 targetPosition)
{
	unsigned int ticksIWant = 300;
	time_type durationIWant = 10.0f;

	numTicks.push_back(ticksIWant);
	animationDuration.push_back(durationIWant);

	//unsigned int numAnimation = getNumOfAnimations();
	// Create all keyframes
	for (unsigned int i = 0; i < allNodes.size(); i++)
	{
		for (unsigned int j = 0; j < ticksIWant; j++)
		{
			// Yeah I'll implement it later
		}
	}
}
// Some modelling software handle UV coordinates differently
// Flip it if necessary
void Mesh::flipUV()
{
	for (unsigned int i = 0; i < texCoord.size(); i++)
	{
		for (unsigned int j = 0; j < texCoord[i].size(); j++)
		{
			texCoord[i][j].y = 1 - texCoord[i][j].y;
		}
	}
}

// This function loop through the current node to parent node
// and get the global transformation by suming up all the transformation
glm::mat4 Mesh::getGlobalTransformation(MeshNode currentNode)
{
	glm::mat4 transformation(1);
	bool isRoot = false;
	while (!isRoot)
	{
		transformation = currentNode.transformation * transformation;

		if (currentNode.parent == nullptr)
		{
			isRoot = true;
			break;
		}

		currentNode = *currentNode.parent;
	}

	return transformation;
}

void Mesh::resetFrameIndex()
{
	frameIndex = 0;
}


void Mesh::prepForRenderer()
{
	std::vector<std::vector<glm::vec3>> mVertices;
	mVertices.resize(vertices.size());

	std::vector<std::vector<glm::vec3>> mNormals;
	mNormals.resize(normals.size());

	// Apply transformation from nodes
	for (unsigned int i = 0; i < allNodes.size(); i++)
	{
		unsigned int meshIndex = allNodes[i]->meshIndex;

		mVertices[meshIndex].resize(vertices[meshIndex].size());
		mNormals[meshIndex].resize(normals[meshIndex].size());

		if (getNumOfAnimations() < 1)
		{
			glm::mat4 transformation = getGlobalTransformation(*allNodes[i]);

			for (unsigned int j = 0; j < vertices[meshIndex].size(); j++)
			{
				mVertices[meshIndex][j] = glm::vec3(transformation *
					glm::vec4(vertices[meshIndex][j], 1.0));
				mNormals[meshIndex][j] = glm::vec3(transformation *
					glm::vec4(normals[meshIndex][j], 1.0));
			}
		}
		else
		{
			// Loop node
			// Notes: This may not 100% work on skeletal meshes
			glm::mat4 transformation = allNodes[i]->transformation;
			for (unsigned int j = 0;
				j < nodeAnimations[animationIndex].size();
				j++)
			{
				if (allNodes[i]->name
					== nodeAnimations[animationIndex][j]->mNodeName.C_Str())
				{
					//std::cout << "Name: " << allNodes[i]->name << "\n";

					//std::cout << nodeAnimations[animationIndex][j]
					//	->mNodeName.C_Str() << "\n";

					// Interpolate between two keyframe
					float ratio = accumulator / timeStep;

					glm::mat4 matrixFloor
						= nodeAnimTransformation[animationIndex][j][whichTickFloor];

					glm::mat4 matrixCeil
						= nodeAnimTransformation[animationIndex][j][whichTickCeil];

					// Framebased way to update transformation
					//transformation
						//= nodeAnimTransformation[animationIndex][j][frameIndex];

					// Linear interpolation
					transformation
						= (matrixCeil * ratio) + ((1.0f - ratio) * matrixFloor);

					break;
				}
			}

			// Apply Transformation to all vertices of the mesh
			for (unsigned int j = 0; j < vertices[meshIndex].size(); j++)
			{
				mVertices[meshIndex][j] = glm::vec3(transformation *
					glm::vec4(vertices[meshIndex][j], 1.0));

				mNormals[meshIndex][j] = glm::vec3(transformation *
					glm::vec4(normals[meshIndex][j], 1.0));
			}
		}
	}

	for (unsigned int i = 0; i < mVertices.size(); i++)
	{
		//verticesPreped.push_back(mVertices[0][i].x); verticesPreped.push_back(mVertices[0][i].y); verticesPreped.push_back(mVertices[0][i].z);
		for (unsigned int j = 0; j < mVertices[i].size(); j++)
		{
			verticesPreped.push_back(mVertices[i][j].x); verticesPreped.push_back(mVertices[i][j].y); verticesPreped.push_back(mVertices[i][j].z);
		}
	}

	for (unsigned int i = 0; i < mNormals.size(); i++)
	{
		//normalsPreped.push_back(normals[0][i].x); normalsPreped.push_back(normals[0][i].y); normalsPreped.push_back(normals[0][i].z);
		for (unsigned int j = 0; j < mNormals[i].size(); j++)
		{
			normalsPreped.push_back(mNormals[i][j].x); normalsPreped.push_back(mNormals[i][j].y); normalsPreped.push_back(mNormals[i][j].z);
		}
	}

	for (unsigned int i = 0; i < texCoord.size(); i++)
	{
		//texCoordPreped.push_back(texCoord[0][i].x); texCoordPreped.push_back(texCoord[0][i].y);
		for (unsigned int j = 0; j < texCoord[i].size(); j++)
		{
			texCoordPreped.push_back(texCoord[i][j].x); texCoordPreped.push_back(texCoord[i][j].y);
		}
	}


	for (unsigned int i = 0; i < faceIndices.size(); i++)
	{
		//elements.push_back((int)(faceIndices[0][i]));
		for (unsigned int j = 0; j < faceIndices[i].size(); j++)
		{
			elements.push_back((int)(faceIndices[i][j]));
		}
	}
}

unsigned int Mesh::getNumOfAnimations()
{
	return animations.size() / 2;
}

// ========================   IRenderable ======================

void Mesh::getArrayOfVertices(std::vector<float>* v)
{
	std::vector<std::vector<glm::vec3>> mVertices;
	mVertices.resize(vertices.size());

	// Apply transformation from nodes
	for (unsigned int i = 0; i < allNodes.size(); i++)
	{
		if (!allNodes[i]->hasMesh)
		{
			// Get which mesh to transform
			unsigned int meshIndex = allNodes[i]->meshIndex;

			mVertices[meshIndex].resize(vertices[meshIndex].size());

			// Apply Transformation to all vertices of the mesh
			for (unsigned int j = 0; j < vertices[meshIndex].size(); j++)
			{
				mVertices[meshIndex][j] = glm::vec3(allNodes[i]->transformation *
					glm::vec4(vertices[meshIndex][j], 1.0));
			}

			//for (unsigned int j = 0; j < mVertices[meshIndex].size(); j++)
			//{
			//	std::cout << glm::to_string(mVertices[meshIndex][j]) << "\n";
			//}
		}
	}

	for (unsigned int i = 0; i < vertices[0].size(); i++)
	{
		//verticesPreped.push_back(vertices[0][i].x); verticesPreped.push_back(vertices[0][i].y); verticesPreped.push_back(vertices[0][i].z);
		v->push_back(mVertices[0][i].x); v->push_back(mVertices[0][i].y); v->push_back(mVertices[0][i].z);
	}
}


void Mesh::getArrayOfTexCoord(std::vector<float>* tC)
{
	for (unsigned int i = 0; i < texCoord[0].size(); i++)
	{
		tC->push_back(texCoord[0][i].x); tC->push_back(texCoord[0][i].y);
	}

}


void Mesh::getArrayOfNormals(std::vector<float>* n)
{
	for (unsigned int i = 0; i < normals[0].size(); i++)
	{
		n->push_back(normals[0][i].x); n->push_back(normals[0][i].y); n->push_back(normals[0][i].z);
	}
}


void Mesh::getArrayOfElements(std::vector<int>* el)
{
	for (unsigned int i = 0; i < faceIndices[0].size(); i++)
	{
		el->push_back((int)(faceIndices[0][i]));
	}
}
