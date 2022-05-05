#pragma once

#include <glm/glm.hpp>

#include <IRenderable.h>
#include <IComponent.h>
#include <Types.h>

namespace NoxEngine {

	/*
	 * A mock up class for Renderer to render a cube
	 */
	class RenderableComponent : public IRenderable, public IComponent {
		public:

			RenderableComponent(f32 trX = 0.0f, f32 trY = 0.0f, f32 trZ = 0.0f, const String texName = "");
			RenderableComponent(const String texName);

			void displayUI() override; 

			inline const i32 getNumOfVertices() const { return (i32)vertices.size(); }
			inline const i32 getNumOfTexCoord() const { return (i32)texCoords.size(); }
			inline const i32 getNumOfNormals()  const { return (i32)normals.size(); }
			inline const i32 getNumOfFaces()	const { return (i32)faces.size(); }

			// TODO: change to a parameter
			inline const String getNormalTexture()  { return normalTexture; }
			inline const String getAmbientTexture() { return ambientTexture; }

			inline const Array<vec3>&  getVertices () const { return vertices; }
			inline const Array<vec2>&  getTexCoords() const { return texCoords; }
			inline const Array<vec3>&  getNormals  () const { return normals; }
			inline const Array<ivec3>& getFaces    () const { return faces; }
			inline const Array<i32>&   getIndices  () const { return indices; }

			String ambientTexture;
			String normalTexture;

	};
}
