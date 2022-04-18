/* 
 * An interface: comunication with Renderer component of the engine.
 * Allows a class that implements it to be rendered by Renderer
 * 
 * When storing geometric data most formats store separate index lists for dif attributes.
 * OpenGl does not allow multiple index buffers for diff attributes. 
 * Hence, this class should prep the data for the Renderer implemented with OpenGl
 */
#pragma once

#include <vector>
#include <Types.h>

namespace NoxEngine {
	class IRenderable
	{
		public:

			// Functions that are taken by the Renderer to render
			// !!! Do not simply return containers. Preps them for renderer
			virtual i32 getNumOfVertices() = 0;
			virtual i32 getNumOfTexCoord() = 0;
			virtual i32 getNumOfNormals() = 0;
			virtual i32 getNumOfElements() = 0;

			virtual void getArrayOfVertices(Array<f32>* v) = 0;
			virtual void getArrayOfTexCoord(Array<f32>* tC) = 0;
			virtual void getArrayOfNormals (Array<f32>* n) = 0;
			virtual void getArrayOfElements(Array<i32> * el) = 0;

			virtual const char* getNormalTexture() = 0;
			virtual const char* getAmbientTexture() = 0;

			// Stores initial attributes without dublicates
			Array<f32> verticesPreped;
			Array<f32> texCoordPreped;
			Array<f32> normalsPreped;

			Array<i32> elements;
			Array<i32> facesV;
			Array<i32> facesT;
			Array<i32> facesN;

			//std::vector<f32> curMatrices; // Animation matrices
			//std::vector<f32> curVertices; // Vertice Positions after Animation Matrices application

			f32 color[3] = { 0.2f, 0.7f, 0.5f }; // Color
			bool prim_type = 0; // If 0 - triangles, 1 - lines
			bool renderAsDots = 0; // 0 - if not, 1 - if yes
	};
}
