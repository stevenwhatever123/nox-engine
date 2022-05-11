#pragma once

// Core GUI
#include "EngineGUI.h"

// Audio classes
#include <Managers/AudioManager.h>
#include <Core/GameState.h>
#include <Managers/IOManager.h>

namespace NoxEngineGUI {
	void updateAudioPanel(NoxEngine::GameState *state, NoxEngineGUI::GUIParams *params);
}
