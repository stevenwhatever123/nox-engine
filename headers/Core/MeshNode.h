#pragma once
#include <vector>
#include <string>
#include <Components/IRenderable.h>
#include <assimp/scene.h>
#include <Core/Types.h>
#include <Utils/Utils.h>

class MeshNode
{
public:

	MeshNode();
	MeshNode(std::istream& stream);
	~MeshNode();

	// The idle transformation
	const mat4 getTransformation();
	// Frame based transformation
	const mat4 getTransformation(u32 frameIndex, u32 animationIndex);
	// Linear interpolated transformation
	const mat4 getTransformation(u32 frameIndex, u32 animationIndex, time_type accumulator, time_type timeStep, i32 whichTickFloor, i32 whichTickCeil);

	// Global transformation by traversing all parent node and adding them up
	mat4 getGlobalTransformation();
	mat4 getGlobalTransformation(u32 frameIndex, u32 animationIndex);
	mat4 getGlobalTransformation(u32 frameIndex, u32 animationIndex, time_type accumulator, time_type timeStep, i32 whichTickFloor, i32 whichTickCeil);

	void setupEulerAngle();
	void convertEulerAngleToMatrix();
	void updateTransformation();

	void updateMaximumFrame(u32 animationIndex, u32 i);
	void updateAnimationSize(u32 animationIndex, u32 num);
	void insertFrameAfter(u32 animationIndex, u32 selectedFrame);
	void insertFrameBefore(u32 animationIndex, u32 selectedFrame);

	const u32 getNumOfAnimations();
	const bool hasAnimations();
	const bool hasMesh();

public:

	String name;

	MeshNode* parent;

	mat4 transformation;

	Array<u32> meshIndex;

	// There could be more than one child node
	Array<MeshNode> child;

	// Animation clip -> keyframe transformation
	// This is just a copy just in case we need it later
	Array<aiNodeAnim*> nodeAnimations;
	Array<Array<mat4>> nodeAnimTransformation;
	Array<Array<mat4>> nodeAnimTranslationMatrices;

	Array<Array<glm::vec3>> eulerAngleXYZ;
	Array<Array<mat4>> nodeAnimRotationMatrices;
	Array<Array<mat4>> nodeAnimScalingMatrices;

	// A vector storing the maximum frame for every animation
	// AKA where the clip is supposed to end in which frame
	Array<u32> maximumFrame;

	MeshNode* parse(std::ostream& stream);

	void serialize(std::ostream& stream);
	void deserialize(std::istream& stream);

private:

};
