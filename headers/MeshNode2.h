#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <IRenderable.h>

// Compiler is my biggest enemy
#define NOMINMAX
#include <assimp/scene.h>
#undef NOMINMAX

#include <Types.h>
#include <Utils.h>

class MeshNode2
{
public:

	MeshNode2();
	~MeshNode2();

	// The idle transformation
	glm::mat4 getTransformation();
	// Frame based transformation
	glm::mat4 getTransformation(u32 frameIndex, u32 animationIndex);
	// Linear interpolated transformation
	glm::mat4 getTransformation(u32 frameIndex, u32 animationIndex, f32 accumulator, f32 timeStep,
		i32 whichTickFloor, i32 whichTickCeil);

	// Global transformation by traversing all parent node and adding them up
	glm::mat4 getGlobalTransformation();
	glm::mat4 getGlobalTransformation(u32 frameIndex, u32 animationIndex);
	glm::mat4 getGlobalTransformation(u32 frameIndex, u32 animationIndex, f32 accumulator, f32 timeStep,
		i32 whichTickFloor, i32 whichTickCeil);

	void setupEulerAngle();
	void convertEulerAngleToMatrix();
	void updateTransformation();

	u32 getNumOfAnimations();
	bool hasAnimations();
	bool hasMesh();

public:

	std::string name;

	MeshNode2* parent;

	glm::mat4 transformation;

	std::vector<u32> meshIndex;

	// There could be more than one child node
	std::vector<MeshNode2> child;

	// Animation clip -> keyframe transformation
	// This is just a copy just in case we need it later
	std::vector<aiNodeAnim*> nodeAnimations;
	std::vector<std::vector<glm::mat4>> nodeAnimTransformation;
	std::vector<std::vector<glm::mat4>> nodeAnimTranslationMatrices;

	std::vector<std::vector<glm::vec3>> eulerAngleXYZ;
	std::vector<std::vector<glm::mat4>> nodeAnimRotationMatrices;
	std::vector<std::vector<glm::mat4>> nodeAnimScalingMatrices;

private:

};