#pragma once
#include <vector>
#include <glm/glm.hpp>

#include "MeshNode.h"
#include "IRenderable.h"

// Compiler is my biggest enemy
#define NOMINMAX
#include <assimp/scene.h>
#undef NOMINMAX


class Mesh : public IRenderable
{
public:

	Mesh();
	~Mesh();

	void resizeNumOfMeshes(unsigned int i);
	void resizeNumOfAnimations(unsigned int i);
	void createNodeHierarchy(aiNode* aiRootnode, MeshNode* rootNode);
	void importAnimationData(aiScene* scene);

	void getAllNodesReferences();

	void printAllNodes();
	void printAllMeshNodes();

	//void prepForRenderer();

public:

	MeshNode nodeHierarchy;
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
	std::vector<std::vector<glm::mat4>> nodeAnimTransformation;
	std::vector<std::vector<glm::mat4>> nodeAnimTranslationMatrices;
	std::vector<std::vector<glm::mat4>> nodeAnimRotationMatrices;
	std::vector<std::vector<glm::mat4>> nodeAnimScalingMatrices;


	// Implementation of interface IRenderable

	inline int getNumOfVertices() { return vertices[0].size(); }
	inline int getNumOfTexCoord() { return texCoord[0].size(); }
	inline int getNumOfNormals() { return normals[0].size(); }
	inline int getNumOfElements() { return faceIndices[0].size() / 3; }


	void getArrayOfVertices(std::vector<float>* v);
	void getArrayOfTexCoord(std::vector<float>* tC);
	void getArrayOfNormals(std::vector<float>* n);

	void getArrayOfElements(std::vector<int>* el);


	const char* getNormalTexture() { return normTexName; }
	const char* getAmbientTexture() { return ambTexName;}

	const char* ambTexName = "textures/Terracotta_Tiles_002_Base_Color.jpg";
	const char* normTexName = "textures/Terracotta_Tiles_002_Normal.jpg";

private:

	void copyNodesWithMeshes(aiNode* node, MeshNode* targetParent);
	void loopAllNodes(MeshNode node, std::vector<MeshNode>& list);
};

