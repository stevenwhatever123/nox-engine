#include <Mesh2.h>
#include <iostream>
#include <glm/gtx/string_cast.hpp>


using namespace NoxEngine;
using NoxEngineUtils::Logger;

Mesh2::Mesh2() { }
Mesh2::~Mesh2() { }

// ========================   IRenderable ======================

void Mesh2::getArrayOfVertices(std::vector<float>* v)
{
	//std::vector<std::vector<glm::vec3>> mVertices;
	//mVertices.resize(vertices.size());

	//// Apply transformation from nodes
	//for (unsigned int i = 0; i < allNodes.size(); i++)
	//{
	//	if (!allNodes[i]->hasMesh)
	//	{
	//		// Get which mesh to transform
	//		unsigned int meshIndex = allNodes[i]->meshIndex;

	//		mVertices[meshIndex].resize(vertices[meshIndex].size());

	//		// Apply Transformation to all vertices of the mesh
	//		for (unsigned int j = 0; j < vertices[meshIndex].size(); j++)
	//		{
	//			mVertices[meshIndex][j] = glm::vec3(allNodes[i]->transformation *
	//				glm::vec4(vertices[meshIndex][j], 1.0));
	//		}

	//		//for (unsigned int j = 0; j < mVertices[meshIndex].size(); j++)
	//		//{
	//		//	std::cout << glm::to_string(mVertices[meshIndex][j]) << "\n";
	//		//}
	//	}
	//}

	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		//verticesPreped.push_back(vertices[0][i].x); verticesPreped.push_back(vertices[0][i].y); verticesPreped.push_back(vertices[0][i].z);
		v->push_back(vertices[i].x); 
		v->push_back(vertices[i].y);
		v->push_back(vertices[i].z);
	}
}


void Mesh2::getArrayOfTexCoord(std::vector<float>* tC)
{
	for (unsigned int i = 0; i < texCoords.size(); i++)
	{
		tC->push_back(texCoords[i].x); 
		tC->push_back(texCoords[i].y);
	}

}


void Mesh2::getArrayOfNormals(std::vector<float>* n)
{
	for (unsigned int i = 0; i < normals.size(); i++)
	{
		n->push_back(normals[i].x); 
		n->push_back(normals[i].y);
		n->push_back(normals[i].z);
	}
}


void Mesh2::getArrayOfElements(std::vector<int>* el)
{
	for (unsigned int i = 0; i < faceIndices.size(); i++)
	{
		el->push_back((int)(faceIndices[i]));
	}
}