#pragma once

#include <IRenderable.h>
#include <IComponent.h>
#include <IPosition.h>
#include <Types.h>
#include <Utils.h>


namespace NoxEngine {

	/*
	 * A mock up class for Renderer to render a cube
	 */
	class GridObject: public IRenderable, public IPosition {
		public:

			void* CastType(const i32 castID);
			GridObject(vec3 start_point, vec3 end_point, const i32 number_of_rects);
			void prepTheData();

			inline i32 getNumOfVertices() { return i32( points.size() ); }
			inline i32 getNumOfTexCoord() { return i32( texCoords.size()/2 ); }
			inline i32 getNumOfNormals()  { return i32( normals.size()/3 ); }
			inline i32 getNumOfElements()  { return i32( elements.size() ); }

			inline const char* getNormalTexture() { return "assets/meshes/textures/leaves_normal.jpg"; }
			inline const char* getAmbientTexture() { return "assets/meshes/textures/leaves_normal.jpg"; }

			inline void getArrayOfVertices(Array<f32>* v)  { *v = points; }
			inline void getArrayOfTexCoord(Array<f32>* tC) { *tC = texCoords; }
			inline void getArrayOfNormals (Array<f32>* n)  { *n = normals; }
			inline void getArrayOfElements(Array<i32>* el) { *el = elements; }

			Array<f32> points;
			Array<f32> texCoords;
			Array<f32> normals;
			
	};
}
