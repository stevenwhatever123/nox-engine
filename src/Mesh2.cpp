#include <Mesh2.h>
#include <iostream>
#include <glm/gtx/string_cast.hpp>


using namespace NoxEngine;
using NoxEngineUtils::Logger;

Mesh2::Mesh2() : RenderableComponent()
{
	glRenderType = GL_TRIANGLES;
	has_normal = true;
	has_texture = true;
	use_indices = false;
}

Mesh2::~Mesh2() { }

void Mesh2::setTexture(const char* filename)
{
	ambTexName = filename;
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
