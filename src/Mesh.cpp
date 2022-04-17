#include "Mesh.h"
#include <iostream>

#include <glm/gtx/string_cast.hpp>

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