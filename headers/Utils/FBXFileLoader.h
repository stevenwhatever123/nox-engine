#pragma once

#include <assimp/Importer.hpp>
#include <Core/Mesh.h>

namespace NoxEngine {
	const aiScene* readFBX(const char* filename);
};

