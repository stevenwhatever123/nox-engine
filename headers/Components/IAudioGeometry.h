#pragma once

#include <Core/Types.h>

#include <Core/Mesh.h>


namespace NoxEngine {

	// forward declares
	class MeshScene;

	struct IAudioGeometry : IRenderable {

	public:
		enum class Shape {
			Undefined = 0, 
			Plane,
			Box,
			Custom,
			Count
		};

	public:
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

		// registered flag is unused
	};
}
