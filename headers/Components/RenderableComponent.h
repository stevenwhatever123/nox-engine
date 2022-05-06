#pragma once

#include "IRenderable.h"
#include "IComponent.h"
#include <Core/Types.h>

namespace NoxEngine {

	/*
	 * A mock up class for Renderer to render a cube
	 */
	class RenderableComponent : public IRenderable, public IComponent {
		public:

			RenderableComponent(f32 trX = 0.0f, f32 trY = 0.0f, f32 trZ = 0.0f, const String texName = "");
			RenderableComponent(const String texName);

			void displayUI() override; 

			i32 getNumOfVertices() override { return (i32)vertices.size(); }
			i32 getNumOfTexCoord() override { return (i32)texCoords.size(); }
			i32 getNumOfNormals() override { return (i32)normals.size(); }
			i32 getNumOfFaces()   override { return (i32)faces.size(); }

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

			Entity* getParentEntity() override { return parent;};
			void attachedToEntity(Entity* ent) override { parent = ent;};

	};
}
