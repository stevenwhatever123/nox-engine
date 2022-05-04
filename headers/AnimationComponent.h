#pragma once
#include <IAnimation.h>
#include "IComponent.h"
#include "MeshScene.h"
#include "Types.h"

namespace NoxEngine
{
	class AnimationComponent: public IAnimation, public IComponent
	{
		AnimationComponent(const MeshScene& scene, u32 index);

		void update();
		void update(time_type dt);

		void updateCeilAndFloor();

		// Reset the play time
		void resetAnimation();

		//void displayUI() override;

		u32 getNumOfAnimations() const;
		bool hasAnimations();
	};
}