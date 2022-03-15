/***************************************
*
* Audio engine
* Base code was developed by Cody Claborn
* (https://www.codyclaborn.me/tutorials/making-a-basic-fmod-audio-engine-in-c/)
*
***************************************/

#include <AudioManager.h>


//////////////////////////////////
//   CONSTRUCTOR / DESTRUCTOR   //
//////////////////////////////////

AudioManager::AudioManager() {

}
AudioManager::~AudioManager() {

}


//////////////
//   CORE   //
//////////////

void AudioManager::Init() {

	// Initialize FMOD Core API
	coreSystem = NULL;

	// Create the main system object
	FMOD_RESULT result = FMOD::System_Create(&coreSystem);
	errorCheck(result);

	// Initialize FMOD
	// can try FMOD_STUDIO_INIT_LIVEUPDATE
	result = coreSystem->init(512, FMOD_INIT_NORMAL, 0);
	errorCheck(result);
}


void AudioManager::Destroy() {

	// Unload all sounds
	for (auto it = mSounds.begin(), itEnd = mSounds.end(); it != itEnd; ++it) {
		const int nameLen = 64;
		char name[nameLen];

		it->second->getName(name, nameLen);

		std::cout << "Unloading " << name << std::endl;
		UnLoadSound(name);		
	}

	// Shutdown FMOD
	coreSystem->release();
}


void AudioManager::Update() {

	/*   Remove all stopped channels   */
	vector<ChannelMap::iterator> pStoppedChannels;
	for (auto it = mChannels.begin(), itEnd = mChannels.end(); it != itEnd; ++it) {

		bool bIsPlaying = false;
		it->second->isPlaying(&bIsPlaying);
		if (!bIsPlaying) {
			pStoppedChannels.push_back(it);
		}
	}

	for (auto& it : pStoppedChannels) {
		mChannels.erase(it);
	}


	// fmod needs to be updated at once once per game tick
	AudioManager::errorCheck(coreSystem->update());
}


void AudioManager::LoadSound(const string& fileName, bool is3d, bool isLooping, bool isStream) {

	// first check that this sound hasn't been loaded into the sound map
	auto soundItr = this->mSounds.find(fileName);
	if (soundItr != mSounds.end()) return;

	// create a mode object based on the flag parameters
	FMOD_MODE eMode = FMOD_DEFAULT;
	eMode |= is3d		? FMOD_3D : FMOD_2D;								// 2D / 3D sound
	eMode |= isLooping	? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;					// Loop or not
	
	// Stream: To account for slow media that might cause buffer underrun 
	// (skipping / stuttering / repeating blocks of audio) with sounds 
	// created with FMOD_CREATESTREAM, use System::setStreamBufferSize to 
	// increase read ahead.
	eMode |= isStream	? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;	// Stream / load compressed sound into memory

	// more flags: FMOD_NONBLOCKING, FMOD_OPENMEMORY_POINT

	/*   create the sound and store it in the sound map   */
	FMOD::Sound* sound = nullptr;		// sound object
	FMOD_CREATESOUNDEXINFO* exinfo = nullptr;		// extended info
	errorCheck(coreSystem->createSound(fileName.c_str(), eMode, exinfo, &sound));
	if (sound) {
		mSounds[fileName] = sound;
	}
}


void AudioManager::UnLoadSound(const std::string& strSoundName) {

	UnLoadSound(strSoundName.c_str());
}


void AudioManager::UnLoadSound(const char *strSoundName) {

	// If the specified sound isn't loaded, don't do anything
	auto sound = this->mSounds.find(strSoundName);
	if (sound == mSounds.end()) return;

	// free the memory occupied by the sound object
	errorCheck(sound->second->release());
	mSounds.erase(sound);
}


int AudioManager::PlaySounds(const string& strSoundName, const glm::vec3& vPos, float fVolumedB) {

	// Has this sound been loaded?
	auto sound = mSounds.find(strSoundName);

	// Sound hasn't been loaded, so load it
	if (sound == mSounds.end()) {
		LoadSound(strSoundName);
		sound = mSounds.find(strSoundName);

		// if not successfully loaded, return the next available channel ID
		// TODO: ?? doesn't make sense
		if (sound == mSounds.end()) {
			return -1;
		}
	}

	// At this point, the sound has to be successfully loaded.
	// Look up which channel this sound resides in.
	auto channelIdItr = mSoundChannelIds.find(strSoundName);
	int channelId = -1;
	
	// If the sound hasn't been allocated a channel, do so 
	if (channelIdItr == mSoundChannelIds.end()) {

		// get the next available channel id
		channelId = mnNextChannelId++;

		// match this sound name with the channel id
		mSoundChannelIds[strSoundName] = channelId;
		std::cout << "Playing a sound in channel " << channelId << std::endl;

		// Create a new channel for the sound and play it. 
		// Start paused ("true") so we don't get a pop when we change the parameters
		// TODO: Differentiate type of sound with an enum and allocate it to the appropriate channel group
		FMOD::Channel* channel = nullptr;
		FMOD::ChannelGroup* channelGroup = nullptr;
		errorCheck(coreSystem->playSound(sound->second, channelGroup, true, &channel));

		// Channel successfully allocated
		if (channel) {
			FMOD_MODE currMode;
			sound->second->getMode(&currMode);
			if (currMode & FMOD_3D) {
				FMOD_VECTOR position = VectorToFmod(vPos);
				errorCheck(channel->set3DAttributes(&position, nullptr));
			}

			// update parameters and unpause
			errorCheck(channel->setVolume(dbToVolume(fVolumedB)));
			errorCheck(channel->setPaused(false));
			mChannels[channelId] = channel;
		}
	}

	// Otherwise (sound has channel), the channel is already playing, so don't do anything.
	
	
	// return channel id at the end
	return channelId;
}

bool AudioManager::IsPlaying(int nChannelId) const {

	// If this channel does not exist, then it's not playing
	auto channel = mChannels.find(nChannelId);
	if (channel == mChannels.end()) return false;

	// Otherwise, fetch from fmod
	bool isPlaying;
	errorCheck(channel->second->isPlaying(&isPlaying));
	return isPlaying;
}



//////////////////////////
//   SETTER FUNCTIONS   //
//////////////////////////

void AudioManager::Set3dListenerAttributes(const glm::vec3& vPos, const glm::vec3& vVel, const glm::vec3& vForward, const glm::vec3& vUp) {

	// convert to fmod vectors
	FMOD_VECTOR pos = VectorToFmod(vPos);
	FMOD_VECTOR forward = VectorToFmod(vForward);
	FMOD_VECTOR up = VectorToFmod(vUp);
	FMOD_VECTOR vel = {0.0f, 0.0f, 0.0f };   // TODO: Take this into account to enable Doppler

	// create and set the listener
	// TODO: This is for studio API
	//FMOD_3D_ATTRIBUTES listener;
	//listener.position = pos;
	//listener.forward = forward;
	//listener.up = up;
	//listener.velocity = vel;

	errorCheck(coreSystem->set3DListenerAttributes(
		0,			// Listener ID. For singleplayer this is always 0
		&pos,
		&vel,
		&forward,
		&up
	));		// Core API
	//errorCheck(studioSystem->setListenerAttributes(0, &listener));		// studio API
	//std::cout << "Position: " << listener.forward.x << "," << listener.forward.y << "," << listener.forward.z << std::endl;
}

// Global 3D settings
// @param dopplerScale Exaggerate / diminish doppler effect
// @param distanceFactor Set units per meter (e.g. if using feet then 3.28). Only affects doppler - does not affect min/max distance
// @param rolloffScale How fast 3D sounds attenuate using FMOD_3D_LOGROLLOFF
void AudioManager::Set3dSettings(float dopplerScale, float distanceFactor, float rolloffScale) {

	errorCheck(coreSystem->set3DSettings(dopplerScale, distanceFactor, rolloffScale));
}

void AudioManager::SetChannel3dPosition(int nChannelId, const glm::vec3& vPosition) {

	// find the specified channel
	auto channelItr = mChannels.find(nChannelId);
	if (channelItr == mChannels.end()) return;

	// update position
	FMOD_VECTOR position = VectorToFmod(vPosition);
	errorCheck(channelItr->second->set3DAttributes(&position, NULL));
}


void AudioManager::SetChannelVolume(int nChannelId, float fVolumedB) {

	// find the specified channel
	auto channelItr = mChannels.find(nChannelId);
	if (channelItr == mChannels.end()) return;

	// update volume
	errorCheck(channelItr->second->setVolume(dbToVolume(fVolumedB)));
}



//////////////////////////
//   HELPER FUNCTIONS   //
//////////////////////////

float AudioManager::dbToVolume(float dB) {
	return powf(10.0f, 0.05f * dB);
}

float AudioManager::VolumeTodB(float volume) {
	return 20.0f * log10f(volume);
}

FMOD_VECTOR AudioManager::VectorToFmod(const glm::vec3& vPosition) {
	FMOD_VECTOR fVec;
	fVec.x = vPosition.x;
	fVec.y = vPosition.y;
	fVec.z = vPosition.z;
	return fVec;
}

int AudioManager::errorCheck(FMOD_RESULT result, bool bPrint, bool bExit) {
	if (FMOD_OK != result) {

		if (bPrint) {
			std::cout << "FMOD ERROR " << result << std::endl;
		}

		if (bExit) {
			exit(-1);
		}

		return 1;
	}

	return 0;
}