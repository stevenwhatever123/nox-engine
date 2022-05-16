#include <Components/AudioSourceComponent.h>

using namespace NoxEngine;


AudioSourceComponent::AudioSourceComponent() {

	//id = ComponentType::AudioSourceType;

	filePath = "";
	volume = 1.0f;
	is3D = true;
	isLooping = true;
	isStream = false;
	channelId = -1;

	selectedDspFilter = 0;

	loaded = false;
	paused = false;
	stopped = false;
}

void AudioSourceComponent::init(String aFilePath, bool aIs3D, bool aIsLooping, bool aIsStream, float aVolume) {

	filePath = aFilePath;
	is3D = aIs3D;
	isLooping = aIsLooping;
	isStream = aIsStream;
	volume = aVolume;

	loaded = true;
}