#include <Core/Mesh.h>
#include <iostream>
#include <glm/gtx/string_cast.hpp>


using namespace NoxEngine;
using NoxEngineUtils::Logger;

Mesh::Mesh() : RenderableComponent()
{
	glRenderType = GL_TRIANGLES;
	has_normal = true;
	has_texture = true;
	use_indices = false;
}

Mesh::Mesh(const Mesh& other)
{
	glRenderType = other.glRenderType;
	has_normal = other.has_normal;
	has_texture = other.has_texture;
	use_indices = other.use_indices;

	name = other.name;
	hasBones = other.hasBones;

	vertices = other.vertices;
	texCoords = other.texCoords;
	normals = other.normals;
	faces = other.faces;
	indices = other.indices;

	color[0] = other.color[0];
	color[1] = other.color[1];
	color[2] = other.color[2];

	glRenderType = other.glRenderType;

	rendObjId = other.rendObjId;
}

Mesh::Mesh(std::istream& stream)
{
	size_t nameSize;
	stream.read((char*)&nameSize, sizeof(nameSize));
	name.resize(nameSize);
	stream.read((char*)&name[0], nameSize);

	stream.read((char*)&hasBones, sizeof(bool));

	stream.read((char*)&glRenderType, sizeof(GLenum));
	stream.read((char*)&has_normal, sizeof(i8));
	stream.read((char*)&has_texture, sizeof(i8));
	stream.read((char*)&use_indices, sizeof(i8));

	size_t verticesSize;
	stream.read((char*)&verticesSize, sizeof(verticesSize));
	vertices.resize(verticesSize);
	stream.read((char*)&vertices[0], verticesSize * sizeof(vec3));

	size_t texCoordsSize;
	stream.read((char*)&texCoordsSize, sizeof(texCoordsSize));
	texCoords.resize(texCoordsSize);
	stream.read((char*)&texCoords[0], texCoordsSize * sizeof(vec2));

	size_t normalSize;
	stream.read((char*)&normalSize, sizeof(normalSize));
	normals.resize(normalSize);
	stream.read((char*)&normals[0], normalSize * sizeof(vec3));

	size_t faceSize;
	stream.read((char*)&faceSize, sizeof(faceSize));
	faces.resize(faceSize);
	stream.read((char*)&faces[0], faceSize * sizeof(ivec3));

	size_t indicesSize;
	stream.read((char*)&indicesSize, sizeof(indicesSize));
	indices.resize(indicesSize);
	stream.read((char*)&indices[0], indicesSize * sizeof(i32));

	stream.read((char*)&color[0], 3 * sizeof(f32));
}

Mesh::~Mesh() { }

void Mesh::setTexture(const String filename)
{
	ambientTexture = filename;
}

void Mesh::serialize(std::ostream& stream)
{
	std::string nameStr(name);
	size_t nameSize = nameStr.size();
	stream.write((char*)&nameSize, sizeof(nameSize));
	stream.write((char*)nameStr.c_str(), nameSize);

	stream.write((char*)&hasBones, sizeof(bool));

	stream.write((char*)&glRenderType, sizeof(GLenum));
	stream.write((char*)&has_normal, sizeof(i8));
	stream.write((char*)&has_texture, sizeof(i8));
	stream.write((char*)&use_indices, sizeof(i8));

	size_t verticesSize = vertices.size();
	stream.write((char*)&verticesSize, sizeof(verticesSize));
	stream.write((char*)&vertices[0], verticesSize * sizeof(vec3));

	size_t texCoordsSize = texCoords.size();
	stream.write((char*)&texCoordsSize, sizeof(texCoordsSize));
	stream.write((char*)&texCoords[0], texCoordsSize * sizeof(vec2));

	size_t normalSize = normals.size();
	stream.write((char*)&normalSize, sizeof(normalSize));
	stream.write((char*)&normals[0], normalSize * sizeof(vec3));

	size_t faceSize = faces.size();
	stream.write((char*)&faceSize, sizeof(faceSize));
	stream.write((char*)&faces[0], faceSize * sizeof(ivec3));

	size_t indicesSize = indices.size();
	stream.write((char*)&indicesSize, sizeof(indicesSize));
	if(indicesSize > 0)
		stream.write((char*)&indices[0], indicesSize * sizeof(i32));

	stream.write((char*)&color[0], 3 * sizeof(f32));
}

void Mesh::deserialize(std::istream& stream)
{
	size_t nameSize;
	stream.read((char*)&nameSize, sizeof(nameSize));
	name.resize(nameSize);
	stream.read((char*)&name[0], nameSize);

	stream.read((char*)&hasBones, sizeof(bool));

	stream.read((char*)&glRenderType, sizeof(GLenum));
	stream.read((char*)&has_normal, sizeof(i8));
	stream.read((char*)&has_texture, sizeof(i8));
	stream.read((char*)&use_indices, sizeof(i8));

	size_t verticesSize;
	stream.read((char*)&verticesSize, sizeof(verticesSize));
	vertices.resize(verticesSize);
	stream.read((char*)&vertices[0], verticesSize * sizeof(vec3));

	size_t texCoordsSize;
	stream.read((char*)&texCoordsSize, sizeof(texCoordsSize));
	texCoords.resize(texCoordsSize);
	stream.read((char*)&texCoords[0], texCoordsSize * sizeof(vec2));

	size_t normalSize;
	stream.read((char*)&normalSize, sizeof(normalSize));
	normals.resize(normalSize);
	stream.read((char*)&normals[0], normalSize * sizeof(vec3));

	size_t faceSize;
	stream.read((char*)&faceSize, sizeof(faceSize));
	faces.resize(faceSize);
	stream.read((char*)&faces[0], faceSize * sizeof(ivec3));

	size_t indicesSize;
	stream.read((char*)&indicesSize, sizeof(indicesSize));
	indices.resize(indicesSize);
	if(indicesSize > 0)
		stream.read((char*)&indices[0], indicesSize * sizeof(i32));

	stream.read((char*)&color[0], 3 * sizeof(f32));
}
