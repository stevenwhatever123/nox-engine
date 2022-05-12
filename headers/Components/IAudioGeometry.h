#pragma once

#include <Core/Types.h>

#include <Core/Mesh.h>


namespace NoxEngine {

	// forward declares
	class MeshScene;

	class IAudioGeometry : public IRenderable {

		public:
			enum class Shape {
				Undefined = 0, 
				Plane,
				Box,
				Custom,
				Count
			};

			// Geometry ID assigned to this obstacle by the AudioManager
			int geometryId;

			// Shape of the geometry (in the case of Plane / Box, it's auto generated from a mesh)
			Shape shape;

			// Reference of MeshScene (if it was loaded from a FBX file)
			MeshScene* meshScene;

			// Get from Mesh:
			// - vertices
			// - face indices
			// - (number of vertices per face)
			// - TODO: direct occlusion values for each face
			// - TODO: reverb occlusion values for each face

			// Plane vertices from the GUI
			vec3 v1, v2, v3, v4;

			// Flag to indicate whether or not to render this geometry
			bool render;

			// For now, the entire geometry has the same direct/reverb occlusion
			float directOcclusion;
			float reverbOcclusion;


		public:
			virtual void clearMesh() = 0;

			virtual bool loadMesh(MeshScene* meshScene) = 0;
			virtual bool generateBoundingBox(const Array<vec3>& verts, mat4 transformation) = 0;
			virtual bool generatePlane(vec3 _v1, vec3 _v2, vec3 _v3, vec3 _v4, mat4 transformation) = 0;
	};
}
