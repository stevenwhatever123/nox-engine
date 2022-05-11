/* ======================================================================================== */
/* AudioSourceComponent.h                                                             */
/* ======================================================================================== */
#pragma once

#include <Core/Types.h>

#include <Components/IAudioSource.h>
#include <Components/IComponent.h>


namespace NoxEngine {

	class AudioSourceComponent : public IAudioSource, public IComponent {
	public:
		AudioSourceComponent();

		//void displayUI() override;

		void init(String aFilePath, bool aIs3D, bool aIsLooping, bool aIsStream, float aVolume = 1.0f);

	};
}
