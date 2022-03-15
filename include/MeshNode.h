#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

class MeshNode
{
public:

	MeshNode();
	~MeshNode();

public:

	std::string name;

	MeshNode *parent;

	glm::mat4 transformation;

	bool hasMesh = false;

	unsigned int meshIndex;

	// There could be more than one child node
	std::vector<MeshNode> child;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> faceIndices;
	std::vector<glm::vec2> texCoord;

private:

};

