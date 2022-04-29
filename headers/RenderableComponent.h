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

			RenderableComponent(f32 trX, f32 trY, f32 trZ, const char* texName);
			RenderableComponent(const char* texName);
			RenderableComponent();

			void* CastType(const i32 castID);

			inline i32 getNumOfVertices() { return (i32)vertices.size(); }
			inline i32 getNumOfTexCoord() { return (i32)texCoords.size(); }
			inline i32 getNumOfNormals()  { return (i32)normals.size(); }
			inline i32 getNumOfFaces() { return (i32)faces.size(); }


			inline const String getNormalTexture()  { return "assets/meshes/textures/leaves_normal.jpg"; }
			inline const String getAmbientTexture() { return ambTexName; }

			inline const Array<vec3>&  getVertices () const { return vertices; }
			inline const Array<vec2>&  getTexCoords() const { return texCoords; }
			inline const Array<vec3>&  getNormals  () const { return normals; }
			inline const Array<ivec3>& getFaces    () const { return faces; }
			inline const Array<i32>&   getIndices  () const { return indices; }

			const String ambTexName;
			
	};
}
