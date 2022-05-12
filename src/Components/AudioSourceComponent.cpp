#include <Components/AudioSourceComponent.h>

using namespace NoxEngine;


AudioSourceComponent::AudioSourceComponent() {

	//id = ComponentType::AudioSourceType;

	filePath = "";
	volume = 1.0f;
	is3D = false;
	isLooping = false;
	isStream = false;
	channelGroupId = -1;

	loaded = false;
}

void AudioSourceComponent::init(String aFilePath, bool aIs3D, bool aIsLooping, bool aIsStream, float aVolume) {

	filePath = aFilePath;
	is3D = aIs3D;
	isLooping = aIsLooping;
	isStream = aIsStream;
	volume = aVolume;

	loaded = true;
}