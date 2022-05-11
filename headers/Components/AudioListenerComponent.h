#pragma once

#include "Components/IComponent.h"
#include "Components/IAudioListener.h"
#include "Core/Types.h"

namespace NoxEngine {

	class AudioListenerComponent : public IAudioListener, public IComponent {

	public:
		static const ComponentType id = ComponentType::AudioListenerType;

		AudioListenerComponent();

		static void exportLua() { };

		virtual Entity* getParentEntity() override { return parent; };
		virtual void attachedToEntity(Entity* ent) override { parent = ent; };

		// setActive and getActive?
	};
}

