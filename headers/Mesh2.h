#pragma once
#include <MeshNode.h>
#include <RenderableComponent.h>
#include <Types.h>
#include <Utils.h>

namespace NoxEngine {

	class Mesh2 : public RenderableComponent {
	public:
		Mesh2();
		Mesh2(std::istream& stream);
		~Mesh2();


		String name;
		bool hasBones;

		void setTexture(const String filename);

		const String getNormalTexture() { return normalTexture; }
		const String getAmbientTexture() { return ambientTexture; }

		//const char* ambTexName = "S:/Masters/COMP5530M Group Project/Work/code/NoxEngine/assets/meshes/textures/Terracotta_Tiles_002_Base_Color.jpg";
		//String ambTexName;
		//String normTexName;
		
		void* CastType(const i32 castID);

		void serialize(std::ostream& stream);
		void deserialize(std::istream& stream);
	};
}
