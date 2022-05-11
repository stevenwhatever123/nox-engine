#pragma once

#include <glm/glm.hpp>

#include <Components/IRenderable.h>
#include <Components/IComponent.h>
#include <Core/Entity.h>
#include <Core/Types.h>

namespace NoxEngine {

	/*
	 * A mock up class for Renderer to render a cube
	 */
	class RenderableComponent : public IRenderable, public IComponent {
		public:

			RenderableComponent(f32 trX = 0.0f, f32 trY = 0.0f, f32 trZ = 0.0f, const String texName = "");
			RenderableComponent(const String texName);
			
			// copy constructor
			RenderableComponent(const RenderableComponent& other);

			//void displayUI() override; 

			inline const i32 getNumOfVertices() const { return (i32)vertices.size(); }
			inline const i32 getNumOfTexCoord() const { return (i32)texCoords.size(); }
			inline const i32 getNumOfNormals()  const { return (i32)normals.size(); }
			inline const i32 getNumOfFaces()	const { return (i32)faces.size(); }

			// TODO: change to a parameter
			const String getNormalTexture() override { return normalTexture; }
			const String getAmbientTexture() override { return ambientTexture; }

			const Array<vec3>&  getVertices () const override { return vertices; }
			const Array<vec2>&  getTexCoords() const override { return texCoords; }
			const Array<vec3>&  getNormals  () const override { return normals; }
			const Array<ivec3>& getFaces    () const override { return faces; }
			const Array<i32>&   getIndices  () const override { return indices; }

			String ambientTexture;
			String normalTexture;

	};
}
