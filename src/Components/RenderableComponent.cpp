#include <Components/RenderableComponent.h>

using namespace NoxEngine;

RenderableComponent::RenderableComponent(f32 trX, f32 trY, f32 trZ, const String texName):
	ambientTexture(texName),
	normalTexture()
{
	//id = ComponentType::RenderableType;

	has_normal = 1;
	has_texture = 1;
	use_indices = 0;
	// Init data

	vertices = {
		{1.00f, -1.00f, -1.00f},
		{ 1.00f, -1.00f,  1.00f},
		{-1.00f, -1.00f,  1.00f},
		{-1.00f, -1.00f, -1.00f},
		{ 1.00f,  1.00f, -0.99f},
		{ 0.99f,  1.00f,  1.00f},
		{-1.00f,  1.00f,  1.00f},
		{-1.00f,  1.00f, -1.00f}
	};


	normals = {
		{ 0.00f, -1.00f,  0.00f},
		{ 0.00f,  1.00f,  0.00f},
		{ 1.00f,  0.00f,  0.00f},
		{-0.00f,  0.00f,  1.00f},
		{-1.00f, -0.00f, -0.00f},
		{ 0.00f,  0.00f, -1.00f}
	};

	texCoords = {
		{1.000000f, 0.333333f}, //0
		{1.000000f, 0.666667f},
		{0.666667f, 0.666667f},
		{0.666667f, 0.333333f},
		{0.666667f, 0.000000f}, //4
		{0.000000f, 0.333333f},
		{0.000000f, 0.000000f},
		{0.333333f, 0.000000f},
		{0.333333f, 1.000000f},
		{0.000000f, 1.000000f},
		{0.000000f, 0.666667f},
		{0.333333f, 0.333333f},
		{0.333333f, 0.666667f},
		{1.000000f, 0.000000f}
	};

	faces = {
		{1,2,3},
		{7,6,5},
		{4,5,1},
		{5,6,2},
		{2,6,7},
		{0,3,7},
		{0,1,3},	
		{4,7,5},
		{0,4,1},
		{1,5,2},
		{3,2,7},
		{4,0,7}
	};

	glRenderType = GL_TRIANGLES;

}

RenderableComponent::RenderableComponent(const String texName):
	ambientTexture(texName),
	normalTexture()
{
	//id = ComponentType::RenderableType;
}

RenderableComponent::RenderableComponent(const RenderableComponent &other) :
	ambientTexture(other.ambientTexture),
	normalTexture(other.normalTexture)
{
	//id = ComponentType::RenderableType;

	vertices	= other.vertices;
	texCoords	= other.texCoords;
	normals		= other.normals;
	faces		= other.faces;
	indices		= other.indices;

	has_texture = other.has_texture;
	has_normal	= other.has_normal;
	use_indices = other.use_indices;

	color[0] = other.color[0];
	color[1] = other.color[1];
	color[2] = other.color[2];

	glRenderType = other.glRenderType;

	rendObjId = -1;
}
