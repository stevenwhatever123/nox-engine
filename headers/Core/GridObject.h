#pragma once

#include <Components/IRenderable.h>
#include <Components/IPosition.h>
#include "Types.h"

namespace NoxEngine {

	/*
	 * A mock up class for Renderer to render a cube
	 */
	class GridObject: public IRenderable, public IPosition {
		public:

			~GridObject();
			GridObject(vec3 start_point, vec3 end_point, const i32 number_of_rects);

			inline i32 getNumOfVertices() { return i32( vertices.size() ); }
			inline i32 getNumOfTexCoord() { return i32( texCoords.size()/2 ); }
			inline i32 getNumOfNormals()  { return i32( normals.size()/3 ); }
			inline i32 getNumOfFaces()  { return i32( faces.size() ); }

			inline const String getNormalTexture() { return "assets/meshes/textures/leaves_normal.jpg"; }
			inline const String getAmbientTexture() { return "assets/meshes/textures/leaves_normal.jpg"; }

			inline const Array<vec3>& getVertices() const { return vertices; }
			inline const Array<vec2>& getTexCoords() const { return texCoords; }
			inline const Array<vec3>& getNormals()  const { return normals; }
			inline const Array<ivec3>& getFaces()   const { return faces; }

			inline const Array<i32>& getIndices()   const { return indices; }

	};
}
