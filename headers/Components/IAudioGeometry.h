#pragma once

#include <Core/Types.h>

#include <Core/Mesh.h>


namespace NoxEngine {

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

		// Get from Mesh:
		// - vertices
		// - face indices
		// - (number of vertices per face)
		// - TODO: direct occlusion values for each face
		// - TODO: reverb occlusion values for each face

		// Plane vertices from the GUI
		vec3 v1, v2, v3, v4;

		bool render;

		// registered flag is unused
	};
}
