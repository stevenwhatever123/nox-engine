#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "MeshNode.h"
#include "IRenderable.h"

// Compiler is my biggest enemy
#define NOMINMAX
#include <assimp/scene.h>
#undef NOMINMAX

typedef double time_type;

class Mesh : public IRenderable
{
public:

	Mesh();
	~Mesh();

	void resizeNumOfMeshes(unsigned int i);
	void resizeNumOfAnimations(unsigned int i);
	void createNodeHierarchy(aiNode* aiRootnode, MeshNode* rootNode);

	void printAllNodes();
	void printAllMeshNodes();

	void update();
	void update(time_type dt);

	void calculateCenterPosition();
	void generateAnimation(glm::vec3 targetPosition);

	glm::mat4 getGlobalTransformation(MeshNode currentNode);

	void flipUV();
	void resetFrameIndex();
	void prepForRenderer();

	unsigned int getNumOfAnimations();

public:

	bool hasBones;
	glm::vec3 centerPosition;

	unsigned int frameIndex = 0;
	unsigned int animationIndex = 0;

	time_type accumulator = 0.0f;
	time_type timeStep = 0.0f;

	int whichTickFloor = 0;
	int whichTickCeil = 0;

	// The root node of the hierarchy, it contains all the child nodes
	MeshNode nodeHierarchy;
	// A reference to all the nodes so we don't have to traversal the tree every time
	std::vector<MeshNode*> allNodes;

	std::vector<std::vector<glm::vec3>> vertices;
	std::vector<std::vector<glm::vec3>> normals;
	std::vector<std::vector<unsigned int>> faceIndices;
	std::vector<std::vector<glm::vec2>> texCoord;

	std::vector<std::vector<aiMaterial>> materials;
	std::vector<std::vector<aiTexture>> textures;
	
	// Animation data
	std::vector<aiAnimation*> animations;
	std::vector<std::vector<aiNodeAnim*>> nodeAnimations;
	// Animation clip -> node -> keyframe transformation
	std::vector<std::vector<std::vector<glm::mat4>>> nodeAnimTransformation;
	std::vector<std::vector<std::vector<glm::mat4>>> nodeAnimTranslationMatrices;
	std::vector<std::vector<std::vector<glm::mat4>>> nodeAnimRotationMatrices;
	std::vector<std::vector<std::vector<glm::mat4>>> nodeAnimScalingMatrices;

	// Animation clip -> node -> numTicks
	std::vector<unsigned int> numTicks;
	// Animation clip -> duration
	std::vector<time_type> animationDuration;


	// Implementation of interface IRenderable

	int getNumOfVertices() { return verticesPreped.size() / 3; }

	int getNumOfTexCoord() { return texCoordPreped.size() / 2; }

	int getNumOfNormals() { return normalsPreped.size() / 3; }


	float* getArrayOfVertices() { return verticesPreped.data(); }

	float* getArrayOfTexCoord() { return texCoordPreped.data(); }

	float* getArrayOfNormals() { return normalsPreped.data(); }


	int getNumOfElements() { return elements.size() / 3; }

	int* getArrayOfElements() { return elements.data(); }


	const char* getNormalTexture() { return texName; }

	const char* getAmbientTexture() { return texName;}

	const char* texName = "textures/leaves_normal.png";

private:

	void traverseTreeStructure(aiNode* node, MeshNode* targetParent);
	void loopAllNodes(MeshNode node, std::vector<MeshNode>& list);
};

