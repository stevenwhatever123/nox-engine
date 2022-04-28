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

			RenderableComponent(f32 trX = 0.0f, f32 trY = 0.0f, f32 trZ = 0.0f, const char * texName = "");
			void displayUI() override; 
			
			void prepTheData();

			inline i32 getNumOfVertices() { return i32( verticesFin.size() / 3 ); }
			inline i32 getNumOfTexCoord() { return i32( texCoordFin.size() / 2 ); }
			inline i32 getNumOfNormals()  { return i32( normalsFin.size() / 3 ); }

			inline f32* getArrayOfVertices() { return verticesFin.data(); }
			inline f32* getArrayOfTexCoord() { return texCoordFin.data(); }
			inline f32* getArrayOfNormals()  { return normalsFin.data(); }
			inline i32 getNumOfElements()    { return (i32)( elements.size() / 3 ); }
			inline i32* getArrayOfElements() { return elements.data(); }

			inline const char* getNormalTexture() { return "assets/meshes/textures/leaves_normal.jpg"; }
			inline const char* getAmbientTexture() { return ambTexName; }

			inline void getArrayOfVertices(Array<f32>* v)  { *v = verticesFin; }
			inline void getArrayOfTexCoord(Array<f32>* tC) { *tC = texCoordFin; }
			inline void getArrayOfNormals (Array<f32>* n)  { *n = normalsFin; }
			inline void getArrayOfElements(Array<i32>* el) { *el = elements; }

			Array<f32> verticesFin;
			Array<f32> texCoordFin;
			Array<f32> normalsFin;
			Array<i32> elements;
			const char* ambTexName;

			std::vector<float> verticesPreped;
			std::vector<float> texCoordPreped;
			std::vector<float> normalsPreped;

			std::vector<int> facesV;
			std::vector<int> facesT;
			std::vector<int> facesN;

	};
}
