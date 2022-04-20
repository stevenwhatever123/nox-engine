#include <MeshNode2.h>

MeshNode2::MeshNode2(){ }

MeshNode2::~MeshNode2(){ }

glm::mat4 MeshNode2::getTransformation()
{
	return transformation;
}

glm::mat4 MeshNode2::getTransformation(u32 frameIndex, u32 animationIndex)
{
	// Frame based way to update transformation
	return nodeAnimTransformation[animationIndex][frameIndex];
}

glm::mat4 MeshNode2::getTransformation(u32 frameIndex, u32 animationIndex, f32 accumulator, f32 timeStep,
	i32 whichTickFloor, i32 whichTickCeil)
{
	for (u32 meshID : meshIndex)
	{
		glm::mat4 defaultTransformation(1);

		if (getNumOfAnimations() < 1)
		{
			getTransformation();
		}

		// Interpolate between two keyframe
		f32 ratio = (f32)(accumulator / timeStep);

		glm::mat4 matrixFloor = nodeAnimTransformation[animationIndex][whichTickFloor];
		glm::mat4 matrixCeil= nodeAnimTransformation[animationIndex][whichTickCeil];

		// Linear interpolation
		return (matrixCeil * (float)ratio) + ((1.0f - (float)ratio) * matrixFloor);
	}
}

u32 MeshNode2::getNumOfAnimations()
{
	return nodeAnimations.size();
}
