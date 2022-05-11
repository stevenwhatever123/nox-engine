/* ======================================================================================== */
/* AudioGeometryComponent.h                                                             */
/* A mesh component that can obstruct audio paths.                                          */
/* ======================================================================================== */
#pragma once

#include <glm/glm.hpp>

#include "Components/RenderableComponent.h"
#include <Components/IAudioGeometry.h>
#include <Components/IComponent.h>
#include <Core/Types.h>


namespace NoxEngine {

	// forward declares
	class MeshScene;

	class AudioGeometryComponent : public IAudioGeometry, public IComponent {
	public:
		static const ComponentType id = ComponentType::AudioGeometryType;

		AudioGeometryComponent();

		//void displayUI() override;

		void clearMesh() override;

		bool loadMesh(MeshScene* meshScene) override;
		bool generateBoundingBox(const Array<vec3>& verts) override;
		bool generatePlane(vec3 _v1, vec3 _v2, vec3 _v3, vec3 _v4) override;

		inline const Array<vec3>&  getVertices() const override { return vertices; }
		inline const Array<vec2>& getTexCoords() const override { return texCoords; }
		inline const Array<vec3>& getNormals()   const override { return normals; }
		inline const Array<ivec3>& getFaces   () const override { return faces; } // Faces are for creating FMOD geometry (expect triangles)
		inline const Array<i32>&   getIndices () const override { return indices; } // Indices are for rendering (draw lines)

		// Unused
		inline const i32 getNumOfVertices()      const override { return (i32)vertices.size(); }
		inline const i32 getNumOfFaces()         const override { return (i32)faces.size(); }
		inline const i32 getNumOfTexCoord()      const override { return (i32)texCoords.size(); }
		inline const i32 getNumOfNormals()       const override { return (i32)normals.size(); }
		inline const String getNormalTexture()         override { return ""; }
		inline const String getAmbientTexture()        override { return ""; }


	private:
		bool generateIndicesFromTriangles(const Array<ivec3>& _faces);
	};
}
