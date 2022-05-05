#pragma once
#include <IAnimation.h>
#include "IComponent.h"
#include "MeshScene.h"
#include <MeshNode2.h>
#include "Types.h"

namespace NoxEngine
{
	class AnimationComponent: public IAnimation, public IComponent
	{
	public:
		AnimationComponent();
		AnimationComponent(const MeshScene& scene, MeshNode2* node);

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
	};
}