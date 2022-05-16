#pragma once

#include <fmod.hpp>
#include <fmod_studio.hpp>
#include <fmod_studio_common.h>
#include <fmod_dsp_effects.h>	// include this for DSP effect enums

#include <Core/Types.h>

//using namespace NoxEngine;

// forward declare
//class NoxEngine::AudioManager;


////////////////////////
/*   Helper structs   */
////////////////////////

// Supported DSP types
enum DSP_TYPE {
	Distortion = 0,
	Echo,
	Fader,
	Flange,
	Limiter,
	PitchShift,
	Tremolo,
	SupportedDSPTypeCount
};

struct DSP {
	//friend class NoxEngine::AudioManager;

public:
	int dspId;
	DSP_TYPE type;

	// DSP filter enum -> parameter value
	std::map< u32, float > params;

//protected:
	FMOD::DSP* dsp;
};

extern std::map<DSP_TYPE, const char*> kDSPNamesMap;

// Conversion from supported DSP type to FMOD internal DSP type
extern std::map<DSP_TYPE, FMOD_DSP_TYPE> kDSPTypeMap;


///////////////////
/*   FMOD Core   */
///////////////////

// Sound filepath -> Sound object
typedef std::map<String, FMOD::Sound*> SoundMap;

// Channel id (monotonically increasing) -> Channel object
typedef std::map<int, FMOD::Channel*> ChannelMap;

// Sound filepath -> Channel id
typedef std::map<String, int> SoundChannelIdMap;

// TODO: ChannelGroup...

// DSP id (monotonically increasing) -> DSP object
typedef std::map<int, DSP*> DSPMap;

typedef u32 DspId;
typedef u32 ChannelId;
typedef u32 ChannelGroupId;


/////////////////////
/*   FMOD Studio   */
/////////////////////

typedef std::map<String, FMOD::Studio::EventInstance*> EventMap;
typedef std::map<String, FMOD::Studio::Bank*> BankMap;
