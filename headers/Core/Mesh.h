#pragma once

#include <Components/RenderableComponent.h>
#include "Types.h"
#include <Utils/Utils.h>

namespace NoxEngine {

	class Mesh : public RenderableComponent {
	public:
		Mesh();
		~Mesh();

		String name;
		bool hasBones;

		void setTexture(const String filename);

		const String getNormalTexture() { return normTexName; }
		const String getAmbientTexture() { return ambTexName; }

		String ambTexName;
		String normTexName;
	
	};
}
