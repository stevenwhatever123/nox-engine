#pragma once

#include <map>
#include <glm/glm.hpp>

#include <fmod.hpp>
#include <fmod_common.h>
#include <fmod_errors.h>
#include <fmod_studio.hpp>
#include <fmod_studio_common.h>


#include <Core/Types.h>
#include "Singleton.h"

namespace NoxEngine {

	class AudioManager : public Singleton<AudioManager> {
		friend class Singleton<AudioManager>;

	private:
		// FMOD Objects
		FMOD::System* coreSystem;
		FMOD::Studio::System* studioSystem;

		i32 mnNextChannelId;

		// FMOD Core API
		typedef std::map<String, FMOD::Sound*> SoundMap;
		typedef std::map<i32, FMOD::Channel*> ChannelMap;
		typedef std::map<String, i32> SoundChannelIdMap;
		SoundMap mSounds;
		ChannelMap mChannels;
		SoundChannelIdMap mSoundChannelIds;

		// FMOD Studio API
		typedef std::map<String, FMOD::Studio::EventInstance*> EventMap;
		typedef std::map<String, FMOD::Studio::Bank*> BankMap;
		BankMap mBanks;
		EventMap mEvents;

	protected:
		AudioManager() {};
		~AudioManager() {};

	public:
		// dummy constructors and destroyers
		// Initialize
		void Init();

		// update loop
		void Update();

		// shutdown fmod and free all resources
		void Destroy();

		// check whether the result is FMOD_OK or not
		// TODO: change to throw std::exception
		static i32 errorCheck(FMOD_RESULT result, bool bPri32 = true, bool bExit = false);

		/*   Core API Functions   */
		void LoadSound(const String& strSoundName, bool is3d = true, bool isLooping = false, bool isStream = false);
		void UnLoadSound(const String& strSoundName);
		void UnLoadSound(const char* strSoundName);
		i32 PlaySounds(const String& strSoundName, const vec3& vPos = vec3{ 0, 0, 0 }, f32 fVolumedB = 0.0f);

		void SetChannel3dPosition(i32 nChannelId, const vec3& vPosition);
		void SetChannelVolume(i32 nChannelId, f32 fNormalizedVolume);
		// Global 3D settings
		void Set3dSettings(f32 dopplerScale, f32 distanceFactor, f32 rolloffScale);
		// Listener position and orientation. TODO: Use velocity != 0 to enable doppler
		void Set3dListenerAttributes(const vec3& vPos, const vec3& vVel, const vec3& vForward, const vec3& vUp);

		bool IsPlaying(i32 nChannelId) const;
		//i32 GetChannelId(const string& strSoundName);

		/*   Studio API Functions   */
		void LoadBank(const  String& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
		void LoadEvent(const String& strEventName);
		void PlayEvent(const String& strEventName);
		void StopChannel(i32 nChannelId);
		void StopEvent(const String& strEventName, bool bImmediate = false);
		void GetEventParameter(const String& strEventName, const String& strEventParameter, f32* parameter);
		void SetEventParameter(const String& strEventName, const String& strParameterName, f32 fValue);
		void StopAllChannels();
		bool IsEventPlaying(const String& strEventName) const;

		/*   Helpers   */
		f32 dbToVolume(f32 dB);
		f32 VolumeTodB(f32 volume);
		static FMOD_VECTOR VectorToFmod(const vec3& vPosition);
	};

}
