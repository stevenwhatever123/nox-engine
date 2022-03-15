#pragma once

#include <vector>

#include "IRenderable.h"
/*
* A mock up class for Renderer to render a cube
*/
class RenderableComp : public IRenderable

{
public:

	RenderableComp(float trX, float trY, float trZ, const char * texName)
	{

		ambTexName = texName;

		// Init data
		verticesPreped = {
			1.000000, -1.000000, -1.000000,
			1.000000, -1.000000, 1.000000,
			-1.000000, -1.000000, 1.000000,
			-1.000000, -1.000000, -1.000000,
			1.000000, 1.000000, -0.999999,
			0.999999, 1.000000, 1.000001,
			-1.000000, 1.000000, 1.000000,
			-1.000000, 1.000000,-1.000000
		};

		normalsPreped = {
			0.000000, - 1.000000, 0.000000,
			0.000000, 1.000000, 0.000000,
			1.000000, 0.000000, 0.000000,
			- 0.000000, 0.000000, 1.000000,
			- 1.000000, - 0.000000, - 0.000000,
			0.000000, 0.000000, - 1.000000
		};


		//texCoord = {

		//	1.000000, 0.333333, //0
		//	1.000000, 0.666667,
		//	0.666667, 0.666667,
		//	0.666667, 0.333333,
		//	0.666667, 0.000000, //4
		//	0.000000, 0.333333,
		//	0.000000, 0.000000,
		//	0.333333, 0.000000,
		//	0.333333, 1.000000,
		//	0.000000, 1.000000,
		//	0.000000, 0.666667,
		//	0.333333, 0.333333,
		//	0.333333, 0.666667,
		//	1.000000, 0.000000
		//
		//};


		texCoordPreped = {

			1.000000, 0.333333, //0
			1.000000, 0.666667,
			0.666667, 0.666667,
			0.666667, 0.333333,
			0.666667, 0.000000, //4
			0.000000, 0.333333,
			0.000000, 0.000000,
			0.333333, 0.000000,
			0.333333, 1.000000,
			0.000000, 1.000000,
			0.000000, 0.666667,
			0.333333, 0.333333,
			0.333333, 0.666667,
			1.000000, 0.000000

		};




		facesV = {
			1,2,3,
			7,6,5,
			4,5,1,
			5,6,2,
			2,6,7,
			0,3,7,
			0,1,3,	
			4,7,5,
			0,4,1,
			1,5,2,
			3,2,7,
			4,0,7
		};



		facesN = {
			0,0,0,
			1,1,1,
			2,2,2,
			3,3,3,
			4,4,4,
			5,5,5,
			0,0,0,
			1,1,1,
			2,2,2,
			3,3,3,
			4,4,4,
			5,5,5			
		};


		facesT = {
			0,1,2,
			0,3,4,
			5,6,7,
			7,4,3,
			8,9,10,
			11,12,10,
			3,0,2,
			13,0,4,
			11,5,7,
			11,7,3,
			12,8,10,
			5,11,10

		};


		for (int i = 0; i < verticesPreped.size()/3; i++)
		{
			verticesPreped[i * 3	  ] += trX;
			verticesPreped[i * 3 + 1] += trY;
			verticesPreped[i * 3 + 2] += trZ;
		}



		prepTheData();


	}


	void prepTheData()
	{
		// Add the very first ind
		// THERE IS A LOT O FTHEM
		verticesFin.push_back(verticesPreped[facesV[0] * 3]); verticesFin.push_back(verticesPreped[facesV[0] * 3 + 1]); verticesFin.push_back(verticesPreped[facesV[0] * 3 + 2]);
		texCoordFin.push_back(texCoordPreped[facesT[0] * 2]); texCoordFin.push_back(texCoordPreped[facesT[0] * 2 + 1]);
		normalsFin.push_back(normalsPreped[facesN[0] * 3]); normalsFin.push_back(normalsPreped[facesN[0] * 3 + 1]); normalsFin.push_back(normalsPreped[facesN[0] * 3 + 2]);
		elements.push_back(0);

		for (int i = 1; i < facesV.size(); i++)
		{
			// Take the values of each attribute at the i-th index buffer
			glm::vec3 theV(verticesPreped[facesV[i]*3], verticesPreped[facesV[i] * 3 + 1], verticesPreped[facesV[i] * 3 + 2]);
			glm::vec2 theT(texCoordPreped[facesT[i]*2], texCoordPreped[facesT[i] * 2 + 1]);
			glm::vec3 theN(normalsPreped[facesN[i]*3], normalsPreped[facesN[i] * 3 + 1], normalsPreped[facesN[i]*3 + 2]);

			bool isStored = false;
			for (int j = 0; j < elements.size(); j++)
			{
				// Take the values of each attribute at the j-th final index buffer
				glm::vec3 thatV(verticesFin[elements[j] * 3], verticesFin[elements[j] * 3+1], verticesFin[elements[j] * 3+2]);
				glm::vec2 thatT(texCoordFin[elements[j] * 2], texCoordFin[elements[j] * 2 + 1]);
				glm::vec3 thatN(normalsFin[elements[j] * 3], normalsFin[elements[j] * 3 + 1], normalsFin[elements[j] * 3 + 2]);

				// Compare the and that (the values)
				// If stored, add index of the stored one in index buffer
				if (theV == thatV && theT == thatT && theN == thatN)
				{
					int test = elements[j];
					elements.push_back(elements[j]);
					isStored = true;
					break;
				}
			}

			// If not stored, add the respective attributes to their container
			if (!isStored)
			{
				verticesFin.push_back(theV.x); verticesFin.push_back(theV.y); verticesFin.push_back(theV.z);
				texCoordFin.push_back(theT.x); texCoordFin.push_back(theT.y);
				normalsFin.push_back(theN.x); normalsFin.push_back(theN.y); normalsFin.push_back(theN.z);

				// and add new index to the elements 
				elements.push_back(verticesFin.size() / 3 - 1);
			}

		
		}

		int k = 0;
	}


	int getNumOfVertices() { return verticesFin.size() / 3; }

	int getNumOfTexCoord() { return texCoordFin.size() / 2; }

	int getNumOfNormals() { return normalsFin.size() / 3; }


	float* getArrayOfVertices() { return verticesFin.data(); }

	float* getArrayOfTexCoord() { return texCoordFin.data(); }

	float* getArrayOfNormals() { return normalsFin.data(); }


	int getNumOfElements() { return elements.size() / 3; }

	int* getArrayOfElements() { return elements.data(); }


	virtual const char* getNormalTexture() { return "textures/leaves_normal.jpg"; }

	virtual const char* getAmbientTexture() { return ambTexName; }


	std::vector<float> verticesFin;
	std::vector<float> texCoordFin;
	std::vector<float> normalsFin;


	std::vector<int> elements;

	const char* ambTexName;





};