#pragma once
#include <Components/RenderableComponent.h>
#include <Core/Types.h>
#include <Utils/Utils.h>

namespace NoxEngine {

	class Mesh : public RenderableComponent {
	public:
		Mesh();
		Mesh(std::istream& stream);
		~Mesh();


		String name;
		bool hasBones;

		void setTexture(const String filename);

		const String getNormalTexture() { return normalTexture; }
		const String getAmbientTexture() { return ambientTexture; }
		
		void serialize(std::ostream& stream);
		void deserialize(std::istream& stream);
	};
}
