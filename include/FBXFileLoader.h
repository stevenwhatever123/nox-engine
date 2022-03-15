#pragma once
#include <assimp/Importer.hpp>

#include "Mesh.h"

namespace FBXFileLoader
{
	const aiScene* readFBX(const char* filename);
	Mesh * getMesh(const aiScene* pScene);
};

