#pragma once

// Core GUI
#include "EngineGUI.h"

// Audio classes
#include <AudioManager.h>
#include <GameState.h>
#include <IOManager.h>

namespace NoxEngineGUI {
	void updateAudioPanel(NoxEngine::GameState *state, NoxEngineGUI::GUIParams *params);
}
