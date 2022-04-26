#include <Mesh2.h>
#include <iostream>
#include <glm/gtx/string_cast.hpp>


using namespace NoxEngine;
using NoxEngineUtils::Logger;

Mesh2::Mesh2() : RenderableComponent()
{
	
}

Mesh2::~Mesh2() { }

void Mesh2::setTexture(const char* filename)
{
	ambTexName = filename;
}

void Mesh2::prepTheData()
{
	for (u32 i = 0; i < vertices.size(); i++)
	{
		verticesPreped.push_back(vertices[i].x);
		verticesPreped.push_back(vertices[i].y);
		verticesPreped.push_back(vertices[i].z);

		normalsPreped.push_back(normals[i].x);
		normalsPreped.push_back(normals[i].y);
		normalsPreped.push_back(normals[i].z);

		texCoordPreped.push_back(texCoords[i].x);
		texCoordPreped.push_back(texCoords[i].y);
	}

	for (u32 i = 0; i < faceIndices.size(); i++)
	{
		facesV.push_back(faceIndices[i]);
		facesN.push_back(faceIndices[i]);
		facesT.push_back(faceIndices[i]);
	}
}

// ========================   IRenderable ======================

void Mesh2::getArrayOfVertices(std::vector<float>* v)
{
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
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

void* Mesh2::CastType(const int castID)
{
	switch (castID)
	{
	case 2:
		return static_cast<IRenderable*>(this); break;
	case 1:
		return static_cast<IComponent*>(this); break;
	default:
		return nullptr;
	}
}