/* ======================================================================================== */
/* AudioManager.h																			*/
/*                                                                                          */
/* A basic audio engine that uses FMOD as the middleware/backend - essentially              */
/* a wrapper for simpler access/modification to sounds in the game engine.                  */
/*                                                                                          */
/* This is built upon Cody Claborn's tutorial from											*/
/* https://codyclaborn.me/tutorials/making-a-basic-fmod-audio-engine-in-c/ .                */
/* License: MIT License.                                                                    */
/*                                                                                          */
/* DSP effects and ChannelGroups are introduced in Guy Somberg's book:			            */
/* Game Audio Programming, Chapter 6.                                                       */
/*                                                                                          */
/* Geometry occlusion (audio ray casting) is introduced in Guy Somberg's book:              */
/* Game Audio Programming 2, Chapter 8.                                                     */
/* More information can be found in Chapter 17 and 18 (for further development).            */
/*                                                                                          */
/* Some low-level desirable functions:														*/
/* https://github.com/Flix01/imgui/blob/imgui_with_addons/addons/imguiyesaddons/imguisoloud.h */
/*                                                                                          */
/* @author: Chan Wai Lou															        */
/* ======================================================================================== */

#pragma once

#include <Core/Types.h>
#include <fmod.hpp>
#include <fmod_studio.hpp>
#include <fmod_studio_common.h>
#include <fmod_dsp_effects.h>	// include this for DSP effect enums

#include "Singleton.h"

#define MAX_CHANNELS 512


namespace NoxEngine {
	
	// Forward declares
	class Mesh;
	class Entity;
	class ITransform;
	class IAudioSource;


	struct DSP {
		friend class AudioManager;

	public:
		DspId dspId;
		ChannelId channelId;

	protected:
		FMOD::DSP* effect;
	};


	class AudioManager : public Singleton<AudioManager> {

		friend class Singleton<AudioManager>;

	private:
		// FMOD Objects
		FMOD::System* coreSystem;
		FMOD::Studio::System* studioSystem;

		int mnNextChannelId;
		int nextDSPId;

		///////////////////
		/*   FMOD Core   */
		///////////////////
		
		// Sound filepath -> Sound object
		typedef std::map<String, FMOD::Sound*> SoundMap;
		SoundMap mSounds;

		// Channel id (monotonically increasing) -> Channel object
		typedef std::map<int, FMOD::Channel*> ChannelMap;
		ChannelMap mChannels;

		// Sound filepath -> Channel id
		typedef std::map<String, int> SoundChannelIdMap;
		SoundChannelIdMap mSoundChannelIds;

		// TODO: ChannelGroup...

		// DSP	
		// Array of DSP filters, intended to be retrieved by index
		Array<FMOD::DSP*> dsps;

		// Channel -> Chain of DSPs
		typedef std::map<int, Array<int>> ChannelDSPMap;
		ChannelDSPMap mDSPs;

		// Geometry objects for occlusion
		Array<FMOD::Geometry*> geometries;

		/////////////////////
		/*   FMOD Studio   */
		/////////////////////
		typedef std::map<String, FMOD::Studio::EventInstance*> EventMap;
		typedef std::map<String, FMOD::Studio::Bank*> BankMap;
		BankMap mBanks;
		EventMap mEvents;

	protected:
		AudioManager();
		~AudioManager();

	public:
		// dummy constructors and destroyers

		/***
		 * Initialize the audio engine
		 * 
		 * @param maxChannels	Maximum number of virtual channels available for playback
		 * @param maxWorldSize	Maximum size of the world from the centerpoint to the edge using the same units used in other 3D functions.
		 * @param flags			Initialization flags. 
								FMOD_INIT_GEOMETRY_USECLOSEST:	only use occlusion result from closest geometry rather than a summation of all
		 */
		void init(
			float maxChannels = MAX_CHANNELS, 
			float maxWorldSize = 500.0f, 
			FMOD_INITFLAGS flags = FMOD_INIT_NORMAL
		);

		// update loop
		void update();

		// shutdown fmod and free all resources
		void destroy();

		// check whether the result is FMOD_OK or not
		static int ERRCHK(FMOD_RESULT result, bool bPrint = true, bool bExit = true);


		////////////////////////////
		/*   Core API Functions   */
		////////////////////////////
		void loadSound(const IAudioSource *src);
		void loadSound(const String& strSoundName, bool is3d = true, bool isLooping = false, bool isStream = false);
		void unLoadSound(const String& strSoundName);
		void unLoadSound(const char* strSoundName);
		int playSounds(const String& strSoundName, const vec3& vPos = vec3{ 0, 0, 0 }, float fVolumedB = 0.0f);
		
		void setChannel3dPosition(int nChannelId, ITransform *ipos);
		void setChannel3dPosition(int nChannelId, const vec3& vPosition);
		void setChannelVolume(int nChannelId, float fNormalizedVolume);

		/***
		 * Global 3D settings
		 *
		 * @param dopplerScale:		Exaggerate / diminish doppler effect
		 * @param distanceFactor:	Set units per meter (e.g. if using feet then 3.28). Only affects doppler - does not affect min/max distance
		 * @param rolloffScale:		How fast 3D sounds attenuate using FMOD_3D_LOGROLLOFF
		 */
		void set3dSettings(float dopplerScale, float distanceFactor, float rolloffScale);
		
		// Listener position and orientation. TODO: Use velocity != 0 to enable doppler
		void set3dListenerAttributes(const vec3& vPos, const vec3& vVel, const vec3& vForward, const vec3& vUp);

		/*   Occlusion   */
		// Create a Geometry object, return the id
		int createGeometry(
			int nMaxPolygons, int nMaxVertices, 
			const vec3 &pos = vec3{0, 0, 0}, 
			const vec3 &forward = vec3{ 0, 0, 0 },
			const vec3 &scale = vec3{ 0, 0, 0 }
		);

		// Create an occlusion Geometry object from an entity that contains a mesh, return the id
		int createGeometry(Entity* ent);

		// Add a polygon to the Geometry object
		void addPolygon(int nGeometryId, float fDirectOcclusion, float fReverbOcclusion, bool isDoubleSided, int nVertices, const Array<vec3>& vVertices);

		// Set whether a Geometry is active
		void setGeometryActive(int nGeometryId, bool isActive);

		// Place Geometry in world space
		void orientGeometry(int nGeometryId, const vec3& pos, const vec3& forward, const vec3& scale, const vec3& up = vec3{ 0, 0, 0 });

		// Save/load Geoemtry object on disk
		void saveGeometry(int nGeometryId, const String& filePath);
		int loadGeometry(const String& filePath);
		

		/*   DSP Filters   */ 
		DspId createDSP(FMOD_DSP_TYPE type, int channelGroup = 0, int channel = 0, int chainPosition = 0);
		void addDSP();	// add to channel group
		//void addDSP();  // add to DSP input
		

		bool isPlaying(int nChannelId) const;
		//int getChannelId(const string& strSoundName);


		//////////////////////////////
		/*   Studio API Functions   */
		//////////////////////////////
		void loadBank(const  String& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
		void loadEvent(const String& strEventName);
		void playEvent(const String& strEventName);
		void stopChannel(int nChannelId);
		void stopEvent(const String& strEventName, bool bImmediate = false);
		void getEventParameter(const String& strEventName, const String& strEventParameter, float* parameter);
		void setEventParameter(const String& strEventName, const String& strParameterName, float fValue);
		void stopAllChannels();


		bool isEventPlaying(const String& strEventName) const;


		//////////////////////////
		/*   HELPER FUNCTIONS   */
		//////////////////////////
		static float dbToVolume(float dB);
		static float volumeTodB(float volume);
		static FMOD_VECTOR vectorToFmod(const vec3& v);
	};

}
