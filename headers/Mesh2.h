#pragma once
#include <vector>
#include <glm/glm.hpp>

#include <MeshNode.h>
#include <RenderableComponent.h>

// Compiler is my biggest enemy
#include <Types.h>
#include <Utils.h>


namespace NoxEngine {

	class Mesh2 : public RenderableComponent {
	public:
		Mesh2();
		~Mesh2();


		String name;
		bool hasBones;

		Array<Array<vec3>> vertices;
		Array<Array<vec3>> normals;
		Array<Array<ivec3>>faceIndices;
		Array<Array<vec2>> texCoord;

		void setTexture(const char* filename);

		const String getNormalTexture() { return normTexName; }
		const String getAmbientTexture() { return ambTexName; }

		//const char* ambTexName = "S:/Masters/COMP5530M Group Project/Work/code/NoxEngine/assets/meshes/textures/Terracotta_Tiles_002_Base_Color.jpg";
		const char* ambTexName = "C:/Users/Steven/Gitlab/noxengine/assets/meshes/textures/Terracotta_Tiles_002_Base_Color.jpg";
		const char* normTexName = "S:/Masters/COMP5530M Group Project/Work/code/NoxEngine/assets/meshes/textures/Terracotta_Tiles_002_Normal.jpg";
		
		void* CastType(const i32 castID);
	
	};
}
