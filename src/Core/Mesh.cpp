#include <Core/Mesh.h>
#include <iostream>

using namespace NoxEngine;
using NoxEngineUtils::Logger;

Mesh::Mesh() : RenderableComponent()
{
	glRenderType = GL_TRIANGLES;
	has_normal = true;
	has_texture = true;
	use_indices = false;
}

Mesh::~Mesh() { }

void Mesh::setTexture(const String filename)
{
	ambTexName = filename;
}

void* Mesh::CastType(const int castID)
{
	switch (castID)
	{
		case 2: return static_cast<IRenderable*>(this); break;
		case 1: return static_cast<IComponent*>(this); break;
		default: return nullptr;
	}
}
