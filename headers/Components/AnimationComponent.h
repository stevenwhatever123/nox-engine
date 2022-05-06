#pragma once
#include <glm/gtx/euler_angles.hpp>

#include <Components/IAnimation.h>
#include <Components/IComponent.h>
#include <Core/MeshScene.h>
#include <Core/MeshNode.h>
#include "Core/Types.h"

namespace NoxEngine
{
	class AnimationComponent: public IAnimation, public IComponent
	{
	public:
		AnimationComponent();
		AnimationComponent(const MeshScene& scene, MeshNode* node);

		void update();
		void update(time_type dt);

		void setAnimationIndex(u32 num);

		void updateCeilAndFloor();

		// Reset the play time
		void resetAnimation();

		//void displayUI() override;

		u32 getNumOfAnimations() const;
		bool hasAnimations();

		void updateNumTicks(u32 animationIndex, u32 num);
		void insertFrame(u32 animationIndex, u32 selectedFrame);

		void setupEulerAngle();
		void convertEulerAngleToMatrix();
		void updateTransformation();

		mat4 getTransformation();

		void updateMaximumFrame(u32 animationIndex, u32 i);
		void updateAnimationSize(u32 animationIndex, u32 num);
		void insertFrameAfter(u32 animationIndex, u32 selectedFrame);
	};
}