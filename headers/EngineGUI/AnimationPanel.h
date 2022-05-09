#pragma once

// Core GUI
#include "EngineGUI.h"

// Animation classes
// TODO: Add here

// Other classes
#include <Core/MeshScene.h>
#include <Core/GameState.h>

namespace NoxEngineGUI {
	void updateAnimationPanel(NoxEngine::GameState *game_state);
	void updateSequencer(NoxEngine::MeshScene &currentScene);
};
