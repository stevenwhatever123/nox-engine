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

class IRenderable
{
public:

    // Functions that are taken by the Renderer to render
    // !!! Do not simply return containers. Preps them for renderer
    virtual int getNumOfVertices() = 0;
    virtual int getNumOfTexCoord() = 0;
    virtual int getNumOfNormals() = 0;

    virtual float* getArrayOfVertices() = 0;
    virtual float* getArrayOfTexCoord() = 0;
    virtual float* getArrayOfNormals() = 0;

    virtual int getNumOfElements() = 0;
    virtual int* getArrayOfElements() = 0;


    virtual const char* getNormalTexture() = 0;
    virtual const char* getAmbientTexture() = 0;




    //virtual int getNumOfFacesV() = 0;
    //virtual int getNumOfFacesT() = 0;
    //virtual int getNumOfFacesN() = 0;

    //virtual const int* getArrayOfFacesV() = 0;
    //virtual const int* getArrayOfFacesT() = 0;
    //virtual const int* getArrayOfFacesN() = 0;







    //virtual float* getAnimationMatrices(int frame_index) = 0; // Probably will be separated into its own animation part


    // Stores initial attributes without dublicates
    std::vector<float> verticesPreped;
    std::vector<float> texCoordPreped;
    std::vector<float> normalsPreped;

    std::vector<int> elements;
    std::vector<int> facesV;
    std::vector<int> facesT;
    std::vector<int> facesN;

    //std::vector<float> curMatrices; // Animation matrices
    //std::vector<float> curVertices; // Vertice Positions after Animation Matrices application




    float color[3] = { 0.2f, 0.7f, 0.5f }; // Color
    bool prim_type = 0; // If 0 - triangles, 1 - lines
    bool renderAsDots = 0; // 0 - if not, 1 - if yes


};

