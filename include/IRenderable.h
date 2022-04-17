/* 
 * An interface: comunication with Renderer system of the engine.
 * Allows a class that implements this interface to be rendered by Renderer
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

    virtual void getArrayOfVertices(std::vector<float>* v) = 0;
    virtual void getArrayOfTexCoord(std::vector<float>* tC) = 0;
    virtual void getArrayOfNormals(std::vector<float>* n) = 0;

    virtual int getNumOfElements() = 0;
    virtual void getArrayOfElements(std::vector<int> * el) = 0;


    virtual const char* getNormalTexture() = 0;
    virtual const char* getAmbientTexture() = 0;

    //virtual float* getAnimationMatrices(int frame_index) = 0; // Probably will be separated into its own animation part


    // Stores initial attributes without dublicates

    //std::vector<float> curMatrices; // Animation matrices
    //std::vector<float> curVertices; // Vertice Positions after Animation Matrices application


};

