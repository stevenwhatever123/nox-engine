/* ======================================================================================== */
/* AudioGeometryComponent.h                                                             */
/* A mesh component that can obstruct audio paths.                                          */
/* ======================================================================================== */
#pragma once

#include <glm/glm.hpp>

#include <IAudioGeometry.h>
#include <IComponent.h>
#include <Types.h>

#include "RenderableComponent.h"

namespace NoxEngine {

	// forward declares
	class MeshScene;

	class AudioGeometryComponent : public IAudioGeometry, public IComponent {
	public:
		AudioGeometryComponent();

		//void displayUI() override;

		void loadMesh(const MeshScene* meshScene);
		void generateBoundingBox(const Array<vec3>& verts);
		void generatePlane(vec3 _v1, vec3 _v2, vec3 _v3, vec3 _v4);

		inline const i32 getNumOfVertices() const { return vertices.size(); }
		inline const i32 getNumOfFaces()	const { return faces.size(); }

		inline const Array<vec3>&  getVertices() const { return vertices; }

		// Faces are for creating FMOD geometry (expect triangles)
		inline const Array<ivec3>& getFaces   () const { return faces; }

		// Indices are for rendering (draw lines)
		inline const Array<i32>&   getIndices () const { return indices; }


		// Unused
		inline const i32 getNumOfTexCoord() const { return (i32)texCoords.size(); }
		inline const i32 getNumOfNormals()  const { return (i32)normals.size(); }
		inline const Array<vec2>& getTexCoords() const { return texCoords; }
		inline const Array<vec3>& getNormals() const { return normals; }
		inline const String getNormalTexture() { return ""; }
		inline const String getAmbientTexture() { return ""; }


	private:
		void generateIndicesFromTriangles(const Array<ivec3>& _faces);
	};
}
