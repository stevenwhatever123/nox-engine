/***************************************
* 
* Audio engine
* Base code was developed by Cody Claborn
* (https://www.codyclaborn.me/tutorials/making-a-basic-fmod-audio-engine-in-c/)
* 
***************************************/

#pragma once

#include <string>
#include <vector>
#include <map>
#include <iostream>

#include <glm/glm.hpp>

#include <fmod/core/fmod.hpp>
#include <fmod/core/fmod_common.h>
#include <fmod/core/fmod_errors.h>
#include <fmod/studio/fmod_studio.hpp>
#include <fmod/studio/fmod_studio_common.h>

using namespace std;


class AudioManager
{
private:
	// FMOD Objects
	FMOD::System* coreSystem;
	FMOD::Studio::System* studioSystem;

	int mnNextChannelId;
	
	// FMOD Core API
	typedef map<string, FMOD::Sound*> SoundMap;
	typedef map<int, FMOD::Channel*> ChannelMap;
	typedef map<string, int> SoundChannelIdMap;
	SoundMap mSounds;
	ChannelMap mChannels;
	SoundChannelIdMap mSoundChannelIds;

	// FMOD Studio API
	typedef map<string, FMOD::Studio::EventInstance*> EventMap;
	typedef map<string, FMOD::Studio::Bank*> BankMap;
	BankMap mBanks;
	EventMap mEvents;

	// TODO: Maybe add an enum of sound types (music, dialogue, SFX, etc)
	//       Or make these a map and use 1 channel for each type

public:
	// dummy constructors and destroyers
	AudioManager();
	~AudioManager();

	// Initialize
	void Init();

	// update loop
	void Update();

	// shutdown fmod and free all resources
	void Destroy();

	// check whether the result is FMOD_OK or not
	// TODO: change to throw std::exception
	static int errorCheck(FMOD_RESULT result, bool bPrint = true, bool bExit = false);


	/*   Core API Functions   */
	void LoadSound(const string& strSoundName, bool is3d = true, bool isLooping = false, bool isStream = false);
	void UnLoadSound(const string& strSoundName);
	void UnLoadSound(const char* strSoundName);
	int PlaySounds(const string& strSoundName, const glm::vec3& vPos = glm::vec3{ 0, 0, 0 }, float fVolumedB = 0.0f);

	void SetChannel3dPosition(int nChannelId, const glm::vec3& vPosition);
	void SetChannelVolume(int nChannelId, float fVolumedB);
	// Global 3D settings
	void Set3dSettings(float dopplerScale, float distanceFactor, float rolloffScale);
	// Listener position and orientation. TODO: Use velocity != 0 to enable doppler
	void Set3dListenerAttributes(const glm::vec3& vPos, const glm::vec3& vVel, const glm::vec3& vForward, const glm::vec3& vUp);

	bool IsPlaying(int nChannelId) const;
	//int GetChannelId(const string& strSoundName);


	/*   Studio API Functions   */
	void LoadBank(const string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
	void LoadEvent(const string& strEventName);
	void PlayEvent(const string& strEventName);
	void StopChannel(int nChannelId);
	void StopEvent(const string& strEventName, bool bImmediate = false);
	void GetEventParameter(const string& strEventName, const string& strEventParameter, float* parameter);
	void SetEventParameter(const string& strEventName, const string& strParameterName, float fValue);
	void StopAllChannels();


	bool IsEventPlaying(const string& strEventName) const;
	

	/*   Helpers   */
	float dbToVolume(float dB);
	float VolumeTodB(float volume);
	static FMOD_VECTOR VectorToFmod(const glm::vec3& vPosition);
};

